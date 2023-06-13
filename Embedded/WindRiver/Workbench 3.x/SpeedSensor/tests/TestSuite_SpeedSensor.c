#include "cpptest.h"

CPPTEST_CONTEXT("/SpeedSensor/SpeedSensor.c");
CPPTEST_TEST_SUITE_INCLUDED_TO("/SpeedSensor/SpeedSensor.c");

CPPTEST_TEST_SUITE(TestSuite_SpeedSensor);
CPPTEST_TEST(TestSuite_SpeedSensor_test_progStart_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_printSpeed_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_displaySpeed_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_analyzeSpeed_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_calculateSpeed_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_processSpeed_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_signalData_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_scanFrequencySensor_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_sendData_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_createReport_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_addToReportQueue_1);
CPPTEST_TEST(TestSuite_SpeedSensor_test_sendReports_1);
CPPTEST_TEST_SUITE_END();

CPPTEST_TEST_SUITE_REGISTRATION(TestSuite_SpeedSensor);

void TestSuite_SpeedSensor_setUp(void)
{
	dataSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	resultSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	reportSemId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	queueGuardSemId = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
}

void TestSuite_SpeedSensor_tearDown(void)
{
	semDelete(dataSemId);
	semDelete(resultSemId);
	semDelete(reportSemId);
	semDelete(queueGuardSemId);
}

/* CPPTEST_TEST_CASE_BEGIN test_progStart_1 */
/* CPPTEST_TEST_CASE_CONTEXT void progStart(void) */
void TestSuite_SpeedSensor_test_progStart_1()
{
	/* Test program startup and shutdown */
	progStart();
	progStop();
}
/* CPPTEST_TEST_CASE_END test_progStart_1 */

/* CPPTEST_TEST_CASE_BEGIN test_printSpeed_1 */
/* CPPTEST_TEST_CASE_CONTEXT void printSpeed(double, double, double) */
void TestSuite_SpeedSensor_test_printSpeed_1()
{
    /* Pre-condition initialization */
    /* Initializing argument 1 (mps) */ 
    double _mps  = cpptestLimitsGetMaxPosDouble();
    /* Initializing argument 2 (mph) */ 
    double _mph  = cpptestLimitsGetMaxPosDouble();
    /* Initializing argument 3 (kmph) */ 
    double _kmph  = cpptestLimitsGetMaxPosDouble();
    
    /* Tested function call */
    printSpeed(_mps, _mph, _kmph);
}
/* CPPTEST_TEST_CASE_END test_printSpeed_1 */

/* CPPTEST_TEST_CASE_BEGIN test_displaySpeed_1 */
/* CPPTEST_TEST_CASE_CONTEXT void displaySpeed(void) */
void TestSuite_SpeedSensor_test_displaySpeed_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable resultSemId */ 
    {
    	semGive(resultSemId);
    }
    /* Initializing global variable mpsSpeed */ 
    {
    	mpsSpeed  = cpptestLimitsGetMaxPosDouble();
    }
    /* Initializing global variable kmphSpeed */ 
    {
    	kmphSpeed  = cpptestLimitsGetMaxPosDouble();
    }
    /* Initializing global variable mphSpeed */ 
    {
    	mphSpeed  = cpptestLimitsGetMaxPosDouble();
    }
    
    /* Tested function call */
    CPPTEST_REGISTER_JMP(displaySpeed());
    
    /* Post-condition check */
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(1.797693e+308, ( mpsSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(1.797693e+308, ( kmphSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(1.797693e+308, ( mphSpeed ));
}
/* CPPTEST_TEST_CASE_END test_displaySpeed_1 */

/* CPPTEST_TEST_CASE_BEGIN test_processSpeed_1 */
/* CPPTEST_TEST_CASE_CONTEXT void processSpeed(void) */
void TestSuite_SpeedSensor_test_processSpeed_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable dataSemId */ 
    {
    	semGive(dataSemId);
    }
    /* Initializing global variable returningFrequency */ 
    {
         returningFrequency  = 0.0;
    }
    /* Initializing global variable mpsSpeed */ 
    {
         mpsSpeed  = 0.0;
    }
    /* Initializing global variable kmphSpeed */ 
    {
         kmphSpeed  = 0.0;
    }
    /* Initializing global variable mphSpeed */ 
    {
    	// This has to be less that SPEED_LIMIT in order to take the desired path,
    	// will be determined by returningFrequency
         mphSpeed  = 0.0;
    }
    /* Initializing global variable pQueueFront */ 
    {
         pQueueFront  = 0 ;
    }
    /* Initializing global variable pQueueBack */ 
    {
         pQueueBack  = 0 ;
    }
    
    /* Tested function call */
    CPPTEST_REGISTER_JMP(processSpeed());
    
    /* Post-condition check */
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(0.000000e+00, ( returningFrequency ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(-1.500000e+08, ( mpsSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(-5.400000e+08, ( kmphSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(-3.375000e+08, ( mphSpeed ));
    CPPTEST_ASSERT_EQUAL(0, ( pQueueFront ));
    CPPTEST_ASSERT_EQUAL(0, ( pQueueBack ));
}
/* CPPTEST_TEST_CASE_END test_processSpeed_1 */

/* CPPTEST_TEST_CASE_BEGIN test_analyzeSpeed_1 */
/* CPPTEST_TEST_CASE_CONTEXT void analyzeSpeed(void) */
void TestSuite_SpeedSensor_test_analyzeSpeed_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable kmphSpeed */ 
    {
         kmphSpeed  = 65;
    }
    /* Initializing global variable mphSpeed */ 
    {
         mphSpeed  = SPEED_LIMIT + 1;
    }
    /* Initializing global variable pQueueFront */ 
    {
         pQueueFront  = 0 ;
    }
    /* Initializing global variable pQueueBack */ 
    {
         pQueueBack  = 0 ;
    }
    
    /* Tested function call */
    analyzeSpeed();
    
    /* Post-condition check */
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(6.500000e+01, ( kmphSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(6.600000e+01, ( mphSpeed ));
    CPPTEST_ASSERT(0 != ( pQueueFront ));
    CPPTEST_ASSERT(0 != ( pQueueBack ));
}
/* CPPTEST_TEST_CASE_END test_analyzeSpeed_1 */

/* CPPTEST_TEST_CASE_BEGIN test_calculateSpeed_1 */
/* CPPTEST_TEST_CASE_CONTEXT void calculateSpeed(void) */
void TestSuite_SpeedSensor_test_calculateSpeed_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable returningFrequency */ 
    {
         returningFrequency  = 4.8e+010;
    }
    /* Initializing global variable mpsSpeed */ 
    {
         mpsSpeed  = 4.8e+010;
    }
    /* Initializing global variable kmphSpeed */ 
    {
         kmphSpeed  = 4.8e+010;
    }
    /* Initializing global variable mphSpeed */ 
    {
         mphSpeed  = 4.8e+010;
    }
    
    /* Tested function call */
    calculateSpeed();
    
    /* Post-condition check */
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(4.800000e+10, ( returningFrequency ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(1.500000e+08, ( mpsSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(5.400000e+08, ( kmphSpeed ));
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(3.375000e+08, ( mphSpeed ));
}
/* CPPTEST_TEST_CASE_END test_calculateSpeed_1 */

/* CPPTEST_TEST_CASE_BEGIN test_scanFrequencySensor_1 */
/* CPPTEST_TEST_CASE_CONTEXT int scanFrequencySensor(double *) */
void TestSuite_SpeedSensor_test_scanFrequencySensor_1()
{
    /* Pre-condition initialization */
    /* Initializing argument 1 (frequency) */ 
    double _frequency_0 [1];
    double * _frequency  = (double * )cpptestMemset((void*)&_frequency_0, 0, (1 * sizeof(double)));
    
    /* Tested function call */
    int _return  = scanFrequencySensor(_frequency);
    
    /* Post-condition check */
    CPPTEST_ASSERT_INTEGER_EQUAL(1, ( _return ));
}
/* CPPTEST_TEST_CASE_END test_scanFrequencySensor_1 */

/* CPPTEST_TEST_CASE_BEGIN test_signalData_1 */
/* CPPTEST_TEST_CASE_CONTEXT void signalData(void) */
void TestSuite_SpeedSensor_test_signalData_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable returningFrequency */ 
    {
         returningFrequency  = 0;
    }
    
    /* Tested function call */
    CPPTEST_REGISTER_JMP(signalData());
    
    /* Post-condition check */
    CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(2.400000e+10, ( returningFrequency ));
}
/* CPPTEST_TEST_CASE_END test_signalData_1 */

/* CPPTEST_TEST_CASE_BEGIN test_sendData_1 */
/* CPPTEST_TEST_CASE_CONTEXT void sendData(void) */
void TestSuite_SpeedSensor_test_sendData_1()
{
	/* Test for NULL SpeedingReport */
	 int _return = sendData(0);

	 /* Post-condition check */
    CPPTEST_ASSERT_INTEGER_EQUAL(0, ( _return ));
}
/* CPPTEST_TEST_CASE_END test_sendData_1 */

/* CPPTEST_TEST_CASE_BEGIN test_addToReportQueue_1 */
/* CPPTEST_TEST_CASE_CONTEXT void addToReportQueue(void) */
void TestSuite_SpeedSensor_test_addToReportQueue_1()
{
	/* Tests for failed call to malloc */
	
    /* Pre-condition initialization */
	/* Initializing global variable kmphSpeed */ 
	{
		 kmphSpeed  = 4.8e+010;
	}
	/* Initializing global variable mphSpeed */ 
	{
		 mphSpeed  = 4.8e+010;
	}
	SpeedingViolation *_report1 = createReport(SPEED_LIMIT, kmphSpeed, mphSpeed);
	SpeedingViolation *_report2 = createReport(SPEED_LIMIT, kmphSpeed, mphSpeed);
	
    /* Tested function call */
	/* We need two reports to trigger both paths */
	addToReportQueue(_report1);
	addToReportQueue(_report2);
	
	/* Clean up */
	free (_report1);
	free (_report2);
}
/* CPPTEST_TEST_CASE_END test_addToReportQueue_1 */

/* CPPTEST_TEST_CASE_BEGIN test_createReport_1 */
/* CPPTEST_TEST_CASE_CONTEXT void createReport(void) */
void TestSuite_SpeedSensor_test_createReport_1()
{
	/* Tests for failed call to malloc */
	
    /* Pre-condition initialization */
	/* Initializing global variable kmphSpeed */ 
	{
		 kmphSpeed  = 4.8e+010;
	}
	/* Initializing global variable mphSpeed */ 
	{
		 mphSpeed  = 4.8e+010;
	}
	
    /* Tested function call */
	SpeedingViolation *_report = createReport(SPEED_LIMIT, kmphSpeed, mphSpeed);

    /* Post-condition check */
    CPPTEST_ASSERT_EQUAL(0, ( _report ));
}
/* CPPTEST_TEST_CASE_END test_createReport_1 */

/* CPPTEST_TEST_CASE_BEGIN test_sendReports_1 */
/* CPPTEST_TEST_CASE_CONTEXT void sendReports(void) */
void TestSuite_SpeedSensor_test_sendReports_1()
{
    /* Pre-condition initialization */
    /* Initializing global variable reportSemId */ 
    {
    	semGive(reportSemId);
    }
    /* Initializing global variable queueGuardSemId */ 
    {
    	semGive(queueGuardSemId);
    }
    /* Initializing global variable pQueueFront */ 
    {
         pQueueFront  = 0 ;
    }
    /* Initializing global variable pQueueBack */ 
    {
         pQueueBack  = 0 ;
    }
    
	/* Initializing global variable kmphSpeed */ 
	{
		 kmphSpeed  = 4.8e+010;
	}
	/* Initializing global variable mphSpeed */ 
	{
		 mphSpeed  = 4.8e+010;
	}
	
    /* Make sure the report queue has some test data in it */
    SpeedingViolation *report = createReport(SPEED_LIMIT, kmphSpeed, mphSpeed);
    addToReportQueue(report);
    
    /* Tested function call */
    CPPTEST_REGISTER_JMP(sendReports());
    
    /* Post-condition check */
    CPPTEST_ASSERT_EQUAL(0, ( pQueueFront ));
    CPPTEST_ASSERT_EQUAL(0, ( pQueueBack ));
}
/* CPPTEST_TEST_CASE_END test_sendReports_1 */
