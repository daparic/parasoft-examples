#include <stdlib.h>
#include "stm32f10x_lib.h"              /* STM32F10x Library Definitions */
#include "lcd.h"                        /* LCD function prototypes */

/* Import functions */
extern void SetupClock     (void);
extern void SetupADC       (void);
extern void SetupUART      (void);
extern void SetupGPIO      (void);
extern void SetupBacklight (void);
extern void SetupWelcomeLCD(void);
extern void _sys_exit(int code);
extern volatile unsigned long  TimeTick;
extern void mainLoopDriver(void);

/*
 * Delay function
 */
void Delay (unsigned long tick) {
  unsigned long timetick;

  timetick = TimeTick;
  while ((TimeTick - timetick) < tick);
}

/*
 * Main function - initializes the hardware and jumps to mainLoopDriver
 */
int main (void) {
  SetupClock();
  SetupADC  ();
  SetupUART ();
  SetupBacklight();
  SetupGPIO();
#ifdef __USE_LCD
  SetupWelcomeLCD();
  Delay(50);
#endif // __USE_LCD
  mainLoopDriver();
  _sys_exit(0);
}
