#include "includes.h"

Int32U CriticalSecCntr;

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

static const Int16U ADC_MaxValue = 4095;

const unsigned Sample_Range = 200;
const int Sample_Offset = -100;

#define LOOP_DLY_100US 450

void Dly100us(void *arg)
{
  Int32U Dly = (Int32U) arg;
  while (Dly--) {
    for (volatile int i = LOOP_DLY_100US; i; i--);
  }
}

void TickHandler(void) {}

static void LCD_clear(Int32U color)
{
  LCD_SET_WINDOW(0, GLCD_HORIZONTAL_SIZE - 1, 0, GLCD_VERTICAL_SIZE - 1);

  for (Int32U i = 0; i < (GLCD_HORIZONTAL_SIZE * GLCD_VERTICAL_SIZE); ++i) {
    LCD_WRITE_PIXEL(color);
  }

  LCD_FLUSH_PIXELS();
}

static void LCD_HBar(Int8U lt, Int8U tp, Int8U rt, Int8U dn, Int8U pts,
  Int32U fg, Int32U bg)
{
  const Int8U l = rt - lt + 1;
  const Int8U w = dn - tp + 1;

  if (pts) {
    LCD_SET_WINDOW(lt, lt + pts - 1, tp, dn);

    for (Int32U i = 0; i < (pts * w); ++i) {
      LCD_WRITE_PIXEL(fg);
    }
  }

  if (pts < l) {
    LCD_SET_WINDOW(lt + pts, rt, tp, dn);

    for (Int32U i = 0; i < ((l - pts) * w); ++i) {
      LCD_WRITE_PIXEL(bg);
    }
  }

  LCD_FLUSH_PIXELS();
}

static Int16U ADC_getValue(void)
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);
  return ADC_GetConversionValue(ADC1);
}

int getSample(void)
{
  return ADC_getValue() * Sample_Range / ADC_MaxValue + Sample_Offset;
}

int isUserBtnDn(void)
{
  return !(B1_PORT->IDR & B1_MASK);
}

int isWkUpBtnDn(void)
{
  return B2_PORT->IDR & B2_MASK;
}

void drawSampleData(const char* msg, int sample)
{
  const unsigned uSample = (unsigned)(sample - Sample_Offset);
  LCD_HBar(5, 90, 124, 105, uSample * 120 / Sample_Range, 0x0FF, 0x000);
  GLCD_SetWindow(5, 108, 131, 131);
  GLCD_TextSetPos(0, 0);
  printf("%13s (%3u%%)", msg, uSample * 100 / Sample_Range);
}

void initIO(void);
void mainLoop(void);

int main()
{
//  Boolean B2_down_old = FALSE;
  Boolean bkLt = TRUE;
  const Int8U bkLtIntens = 0x50;
  const LdcPixel_t bgColor = 0x555;

  initIO();

  // GLCD init
  GLCD_PowerUpInit(0);

  LCD_clear(bgColor);

  GLCD_Backlight(bkLt ? bkLtIntens : BACKLIGHT_OFF);

  GLCD_SetWindow(20, 20, 120, 50);
  GLCD_TextSetPos(0, 0);

  GLCD_SetFont(&Terminal_18_24_12, 0xFB0, bgColor);
  printf("Parasoft");

  GLCD_SetWindow(20, 50, 120, 120);
  GLCD_TextSetPos(0, 0);

  GLCD_SetFont(&Terminal_9_12_6, 0xFFF, bgColor);
  printf("Sensor Example\r\nApplication");

  GLCD_SetFont(&Terminal_6_8_6, 0xFFF, bgColor);

/*
  while (1)
  {
    const Boolean B2_down = isWkUpBtnDn();
    const int sample = getSample();

    drawSampleData("", sample);

    if (!B2_down_old && B2_down) {
      bkLt ^= 1;
      GLCD_Backlight(bkLt ? bkLtIntens : BACKLIGHT_OFF);
    }

    B2_down_old = B2_down;
  }
*/

  mainLoop();

  return 0;
}
