#include <stdio.h>

extern const int STATUS_OK;
//extern const int STATUS_FAILED;
extern const int STATUS_STOPPED;

static const int MAX_NUMBER_OF_SAMPLES = 30;

int readSensor(int* value)
{
    static int v = 0;
    *value = v;
    v++;
    return (v > MAX_NUMBER_OF_SAMPLES) ? STATUS_STOPPED : STATUS_OK;
}

void printMessageImpl(const char* msg, int value)
{
    printf("Value: %d, State: %s\n", value, msg);
    fflush(stdout);
}
