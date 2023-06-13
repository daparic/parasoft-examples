/******************************************************************************/
/* IRQ.C: IRQ Handler                                                         */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2007 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "stm32f10x_lib.h"              /* STM32F10x Library Definitions          */

volatile unsigned char Clock1s;
volatile unsigned long TimeTick;

extern unsigned short int ADC_ConvertedValue;
extern unsigned short CalcAverage (unsigned short);

/*------------------------------------------------------------------------------
  Systick Interrupt Handler
  SysTick interrupt happens every 10 ms
 *------------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static unsigned long ticks;
  if (ticks++ >= 99) {                  /* Set Clock1s to 1 every 1 second    */
    ticks   = 0;
    Clock1s = 1;
  }
  TimeTick++;                          /* used for Delay function             */
}


#ifdef __USE_IRQ

/*------------------------------------------------------------------------------
  ADC1_2 Interrupt Handler
 *------------------------------------------------------------------------------*/
void ADC_IRQHandler (void) {
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET) {  // ADC1 EOC interrupt?
    ADC_ConvertedValue = ADC1->DR;
    ADC_ConvertedValue  = CalcAverage (ADC1->DR);  // calculate average
    /* Clear ADC1 AWD pending interrupt bit */
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}
#endif //__USE_IRQ
