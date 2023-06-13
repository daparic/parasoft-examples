
#ifndef __Sensor_h__
#define __Sensor_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Computation constants
#define MAX_SCALE 100

//Helper constants
#define STATUS_OK  0
#define STATUS_FAILED 1
#define STATUS_STOPPED 2
#define VALUE_LOW 0
#define VALUE_HIGH 1
#define ERROR_CODE 2

unsigned int CalcAverage (unsigned short);
unsigned char readSensorStatus(void);
unsigned int readSensor(unsigned int *);
void reportSensorFailure(void);
void handleSensorValue(unsigned int);

#endif /* __Sensor_h__ */
