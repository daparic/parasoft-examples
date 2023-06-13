/* DESCRIPTION:
 * -----------------------------------------------------------------
 * This module is used as a simple example of a multitasking 
 * application in the form of a VxWorks downloadable kernel module.
 * It simulates a radar gun that detects the speed of an object
 * by using the Doppler Effect. The frequency of the radar gun is 
 * constant, and the data for the returning frequency is generated 
 * by the application. Calculation is then done to determine the 
 * speed of the object. If it determined that the speed is higher
 * than the designated speed limit, then a speeding violation report
 * is "sent off" for further review.
 */

/* Includes */
#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "semLib.h"
#include "taskLib.h"

/* Defines */
#define STACK_SIZE 20000
#define DELAY_TICKS 50   /* Change this to increase/decrease application activity */
#define SUCCESS 1
#define FAILURE 0
#define SENSOR_FAILURE 5 /* Magic number to simulate failure in the sensor        */
#define MS_TO_KMPH 3.6   /* Multiplier for converting meters/second to km/hr      */
#define MS_TO_MPH 2.25   /* Multiplier for converting meters/second to miles/hr   */

/* Constants */
const unsigned long long RADAR_GUN_FREQUENCY = 24000000000ULL; /* 24 GHz                  */
const unsigned long LIGHT_SPEED = 300000000UL;                 /* Speed of light (in m/s) */
const double SPEED_LIMIT = 65.0;                               /* in miles per hour       */


/* task IDs */
TASK_ID tidSignalData;   /* Task for gathering fequency data from sensor       */
TASK_ID tidProcessSpeed; /* Task for processing frequency data from sensor     */
TASK_ID tidDisplaySpeed; /* Task for displaying processed sensor data          */
TASK_ID tidSendReports;  /* Task for sending speeding violations to receiver   */

/* Data globals */
double returningFrequency; /* Stores the simulated return frequency from the radar gun */
double mpsSpeed;           /* Stores speed of object in meters/second                  */
double mphSpeed;           /* Stores speed converted to miles/hour                     */
double kmphSpeed;          /* Stores speed converted to kilometers/hour                */


/* Semaphores */
SEM_ID dataSemId;       /* Given when it is ok to process data                    */
SEM_ID resultSemId;     /* Given based on result accessibility                    */
SEM_ID reportSemId;     /* Given when speeding violation reports are available    */
SEM_ID queueGuardSemId; /* Given when pointer to the report queue can be accessed */

/* Stuctures */
typedef struct speeding_violation
{
	double speed_limit;
	double kphSpeed;
	double mphSpeed;
	struct speeding_violation *pNextNode;
} SpeedingViolation;

SpeedingViolation * pQueueFront  = NULL;	/* front of queue of reports */
SpeedingViolation * pQueueBack  = NULL;	    /* back of queue of reports */

/* forward decls */
void signalData(void);
void processSpeed(void);
void calculateSpeed(void);
void displaySpeed(void);
void sendReports(void);


/* Initializes tasks and semaphores */
STATUS progStart(void)
{
	dataSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	resultSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	reportSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	queueGuardSemId = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	
	tidSignalData = taskSpawn((char*)"frequency_sensor", 200, 0, STACK_SIZE,
							  (FUNCPTR)signalData,0,0,0,0,0,0,0,0,0,0);
	tidProcessSpeed = taskSpawn((char*)"speed_processor", 220, 0, STACK_SIZE,
							  (FUNCPTR)processSpeed,0,0,0,0,0,0,0,0,0,0);
	tidDisplaySpeed = taskSpawn((char*)"speed_displayer", 220, 0, STACK_SIZE,
							  (FUNCPTR)displaySpeed,0,0,0,0,0,0,0,0,0,0);
	tidSendReports = taskSpawn((char*)"report_sender", 180, 0, STACK_SIZE,
							  (FUNCPTR)sendReports,0,0,0,0,0,0,0,0,0,0);
    return (OK);
}

/* Generates the frequency at which the radar signal is returned from the
 * moving object. The returning frequency should increase if the object is
 * moving towards the radar and decrease if moving away. This example always 
 * assumes that the objet is moving towards the radar.
 */
int scanFrequencySensor(double *frequency)
{
    int sensorData = rand() % 10000;
    *frequency = (double)RADAR_GUN_FREQUENCY + sensorData;
    return SUCCESS;
}

/* Task --- Generates sensor data --- */
void signalData(void)
{
    FOREVER {   
    	/* Get the returning frequency from the frequency sensor. */
    	if (scanFrequencySensor(&returningFrequency) == FAILURE) {
            /* If sensor has an error, don't give semaphore, just retry */ 
            printf("The sensor has encountered an error, retrying...\n");
    	} else {
    		semGive(dataSemId);
    		taskDelay(DELAY_TICKS);
    	}
    }
}

/* Uses the gathered frequency to determine the speed */
void calculateSpeed()
{
    /* Doppler Forumla for calculating velocity from frequency change */
    mpsSpeed = ((returningFrequency - RADAR_GUN_FREQUENCY)  / (2 * RADAR_GUN_FREQUENCY)) * LIGHT_SPEED;
    
    /* conversion to alternate units of measurement */
    kmphSpeed = mpsSpeed * MS_TO_KMPH;
    mphSpeed = mpsSpeed * MS_TO_MPH;
}

/* Adds a speeding violation report to the queue */
void addToReportQueue(SpeedingViolation *report)
{
	/* Should check for NULL report */
	semTake (queueGuardSemId, WAIT_FOREVER);
	/* When there is nothing in the queue, front and back should point to the same node */
	if (pQueueFront == NULL) {
		pQueueFront = report;
		pQueueBack = pQueueFront;
	} else {
		/* Add node to the end of the queue */
		pQueueBack->pNextNode = report;
		pQueueBack = report;
	}
	semGive (queueGuardSemId);
	return;
}

/* Create a speeding violation report out of given speed data */
SpeedingViolation* createReport(double speedLimit, double kmph, double mph)
{
	SpeedingViolation *report;

	if ( (report = (SpeedingViolation *) malloc (sizeof (SpeedingViolation))) != NULL)
	{
		report->speed_limit = speedLimit;
		report->kphSpeed = kmph;
		report->mphSpeed = mph;
        report->pNextNode = 0;
	} else {
		printf ("SpeedSensor: Out of Memory.\n");
		taskSuspend (0);
	}
	return report;
}

/* Checks to see if the speed limit has been broken and, if necessary, issues a report
 * for the speeding violation.
 */
void analyzeSpeed()
{
	/* If user changes SPEED_LIMIT constant to other units, make sure
	   to use appropriate global data for comparison (ie. mpsSpeed or kmphSpeed) */
	if (mphSpeed > SPEED_LIMIT) {
		/* create a violation report */
		SpeedingViolation *report = createReport(SPEED_LIMIT, kmphSpeed, mphSpeed);
		addToReportQueue(report);
		semGive(reportSemId);
	}
}

/* Task --- Processes the data received from the sensor --- */
void processSpeed(void)
{   
    FOREVER {
        semTake(dataSemId, WAIT_FOREVER);
        calculateSpeed();
        analyzeSpeed();
        semGive(resultSemId);
    }
}

void printSpeed(double mps, double mph, double kmph)
{
	printf("Object is moving at %.2f m/s, %.2f mi/h, %.2f km/h\n", mps, mph, kmph);
}

/* Task --- Print out the results of the speed calculations --- */
void displaySpeed(void)
{
    FOREVER {
        semTake(resultSemId, WAIT_FOREVER);
        printSpeed(mpsSpeed, mphSpeed, kmphSpeed);
    }
}

/* Deletes first item in queue and frees memory associated with it */
int sendData(SpeedingViolation *report)
{
	if (report) {
		/* Simulated data transmission */
		printf("Sending speeding violation report, culprit going %.2f mi/h, speed limit is %.0f mi/h\n", report->mphSpeed, report->speed_limit);     
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

void removeQueueFront()
{
	/* Should check to make sure pQueueFront is not NULL */
	SpeedingViolation *pTmpNode;
	pTmpNode = pQueueFront;
	pQueueFront = pQueueFront->pNextNode; 
	free (pTmpNode);   
}

/* Task --- Sends all available reports to some receiver --- */
void sendReports(void)
{
	FOREVER {
		semTake (reportSemId, WAIT_FOREVER);	/* Wait for a report */
		semTake (queueGuardSemId, WAIT_FOREVER); /* reserve access to the queue */
		
		while (pQueueFront != NULL) {
			if (sendData(pQueueFront) == FAILURE) {
				printf("Failed to send report data\n");
			} else {
				/* Remove the report from the queue */
				removeQueueFront();
			}
		}
		pQueueBack = NULL;
		
		semGive (queueGuardSemId);   /* release access to the queue */
	}
}


/* Task --- Does cleanup of tasks and semaphores --- */
void progStop(void)
{
	taskDelete(tidSignalData);
	taskDelete(tidProcessSpeed);
	taskDelete(tidDisplaySpeed);
	taskDelete(tidSendReports);
	
	semDelete(dataSemId);
	semDelete(resultSemId);
	semDelete(reportSemId);
	semDelete(queueGuardSemId);

    printf("System shutting down...\n");
    return;
}
