#include "Sensor.h"
#include "lcd.h"

/* Externals */
extern volatile u16 ADC_ConvertedValue;
extern vu8  Clock1s;

/* Globals for messages processing */
static char text[40];
static char** messages = 0;


u32 CalcAverage (u16 val)  {
  static u16 idx      =  0;
  static u16 aval[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  static u32 sum      =  0;

  sum = sum - aval[idx] + val;
  aval[idx] = val;
  idx = (idx +1 ) & 0x1F;

  return (sum >> 5);
}

/*
 * Data acquistion status
 */
u8 readSensorStatus(void)
{
  /* Stop the data acqustion */
  if(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7))
  {
    while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7) == Bit_RESET)
    {
    }
    return STATUS_STOPPED;
  /* Continue collecting data */
  } else {
    return STATUS_OK;
  }
}

/*
 * Sensor interface function
 */
u32 readSensor(u32 * value)
{
    /* Incorrect value - probable sensor error */
    if (ADC_ConvertedValue > 0xfff) {
        return STATUS_FAILED;
	}
    *value =  (int)((float) ADC_ConvertedValue/0xFFF * MAX_SCALE);
	return readSensorStatus();
}

u8 initDisplay()
{
#ifdef __USE_LCD
    LCD_DisplayStringLine(Line0, "   Control Mode     ");
    LCD_DisplayStringLine(Line1, "Current temperature ");
    LCD_SetBackColor(White);                        // Set the Back Color */
    LCD_SetTextColor(Blue);                         // Set the Text Color */
#endif
	return 1;
}

void initialize()
{
	if (messages == 0) {
		messages = (char**)malloc(sizeof(char*) * 3);
		messages[0] = (char*)malloc(128);
		messages[1] = (char*)malloc(128);
		messages[2] = (char*)malloc(128);
		strcpy(messages[VALUE_LOW],  "Sensor Low ");
		strcpy(messages[VALUE_HIGH], "Sensor High");
		strcpy(messages[ERROR_CODE], "Error occured");
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

void printMessage(u8 msgIndex, u32 value)
{
	const char* msg = messages[msgIndex];
#ifdef __USE_LCD
	static u8 i = 0;
    int barLen = 0;
	if (i == 0) { initDisplay();i = 1;}
    barLen = (DISPLAY_WIDTH*value)/MAX_SCALE;
    drawBargraphH (Line6,      0,               barLen, Red  );
    drawBargraphH (Line6, barLen, DISPLAY_WIDTH-barLen, White);
#endif
    if (Clock1s) {
        Clock1s = 0;
        sprintf(text, "%s (%d %%)",msg, value);
#ifdef __USE_LCD
		LCD_DisplayStringLine(Line5, (unsigned char *)"                   ");
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
#endif
		printf("Value: %d, State: %s\n", value, msg);
    }
}

void reportSensorFailure()
{
	initialize();
	printMessage(ERROR_CODE, 0);
	finalize();
}


void handleSensorValue(u32 value)
{
	s8 index;

	initialize();
	index = -1;

	if (value <= 10) {
		index = VALUE_LOW;
	} else if (value <= 20) {
		index = VALUE_HIGH;
	}
	printMessage(index, value);
}

void mainLoopDriver()
{
	u32 sensorValue;
	u8 status = 1;
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
