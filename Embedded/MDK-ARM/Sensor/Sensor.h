#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x_lib.h"

/* Computation constants */
#define MAX_SCALE 100

/* Helper constants */
#define STATUS_OK  0
#define STATUS_FAILED 1
#define STATUS_STOPPED 2
#define VALUE_LOW 0
#define VALUE_HIGH 1
#define ERROR_CODE 2

u32 CalcAverage (u16);
u8 readSensorStatus(void);
u32 readSensor(u32 *);
void reportSensorFailure(void);
void handleSensorValue(u32);
