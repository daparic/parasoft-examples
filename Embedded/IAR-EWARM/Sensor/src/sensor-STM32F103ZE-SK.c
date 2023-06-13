#include <stdio.h>

extern const int STATUS_OK;
extern const int STATUS_FAILED;
extern const int STATUS_STOPPED;

extern const unsigned Sample_Range;
extern const int Sample_Offset;

int getSample(void);
int isUserBtnDn(void);
int isWkUpBtnDn(void);
void drawSampleData(const char* msg, int sample);

int readSensor(int* value)
{
  const int value0 = getSample();
  const int uValue = value0 - Sample_Offset;

  *value = value0;

  if ((uValue < 0) || ((unsigned)uValue > Sample_Range) || isWkUpBtnDn()) {
     return STATUS_FAILED;
  }

  return isUserBtnDn() ? STATUS_STOPPED : STATUS_OK;
}

void printMessageImpl(const char* msg, int value)
{
  drawSampleData(msg, value);
}
