#include "Sensor.h"

//Globals for messages processing
static char text[40];
static char** messages = 0;
const unsigned int MAX_NUMBER_OF_SAMPLES = 30;


/*
 * Sensor interface function
 */
unsigned int readSensor(unsigned int * value)
{
    static int v = 0;
    v++;
    *value = v;
    if (v > MAX_NUMBER_OF_SAMPLES) {
    	return STATUS_STOPPED;
    } else {
    	return STATUS_OK;
    }
}


void initialize() {
	if (messages == 0) {
		messages = (char**) malloc(sizeof(char*) * 3);
		messages[0] = (char*) malloc(128);
		messages[1] = (char*) malloc(128);
		messages[2] = (char*) malloc(128);

		strcpy(messages[VALUE_LOW], "Sensor Low ");
		strcpy(messages[VALUE_HIGH], "Sensor High");
		strcpy(messages[ERROR_CODE], "Error occurred");
	}
}


void finalize()
{
	if (messages) {
		free(messages[0]);
		free(messages[1]);
		free(messages[2]);
		free(messages);
		messages = 0;
	}
}

void printMessage(int msgIndex, unsigned int value) {
	if (messages) {
		const char* msg = messages[msgIndex];
		printf("Value: %d, State: %s\n", value, msg);
		fflush(stdout);
	}
}

void reportSensorFailure()
{
	initialize();
	printMessage(ERROR_CODE, 0);
	finalize();
}


void handleSensorValue(unsigned int value)
{
	int index;

	initialize();
	index = -1;

	if (value <= 10) {
		index = VALUE_LOW;
	} else if (value <= 20) {
		index = VALUE_HIGH;
	}
	printMessage(index, value);
}


