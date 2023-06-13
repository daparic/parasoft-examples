#include <stdlib.h>
#include <string.h>

const int STATUS_OK = 0;
const int STATUS_FAILED = 1;
const int STATUS_STOPPED = 2;

static const int VALUE_LOW_MSG = 0;
static const int VALUE_HIGH_MSG = 1;
static const int ERROR_MSG = 2;

static char** messages = 0;

int readSensor(int* value);
void printMessageImpl(const char* msg, int value);

static void initialize()
{
    if (messages == 0) {
        messages = (char**)malloc(sizeof(char*) * 3);
        messages[0] = (char*)malloc(128);
        messages[1] = (char*)malloc(128);
        messages[2] = (char*)malloc(128);
        strcpy(messages[VALUE_LOW_MSG], "Low");
        strcpy(messages[VALUE_HIGH_MSG], "High");
        strcpy(messages[ERROR_MSG], "Error");
    }
}

static void finalize()
{
    if (messages) {
        free(messages[0]);
        free(messages[1]);
        free(messages[2]);
    }
    free(messages); /* FIX: double free */
}

static void printMessage(int msgIndex, int value)
{
    printMessageImpl(messages[msgIndex], value);
}

static void reportSensorFailure()
{
    finalize(); /* FIX: change order + add initialize() */
//    initialize();
    printMessage(ERROR_MSG, 0);
//    finalize(); /* FIX: change order + add initialize() */
}

static void handleSensorValue(int value)
{
    int index = -1;
    initialize();
    if (value >= 0 && value <= 10) { /* FIX: handle value < 0 */
//    if (value <= 10) { /* FIX: handle value < 0 */
        index = VALUE_LOW_MSG;
    } else if (value > 10 && value <= 20) { /* FIX: handle value > 20 */
//    } else if (value > 10) { /* FIX: handle value > 20 */
        index = VALUE_HIGH_MSG;
    }
    printMessage(index, value);
}

void mainLoop()
{
    int sensorValue;
    int status = 1;
    while (1) {
        status = readSensor(&sensorValue);
        if (status == STATUS_STOPPED) {
            break;
        } else if (status == STATUS_FAILED) {
            reportSensorFailure();
            break;
        }
        handleSensorValue(sensorValue);
    }
    finalize();
}
