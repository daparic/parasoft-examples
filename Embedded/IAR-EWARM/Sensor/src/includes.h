/***************************************************************************
 **
 **    MASTER INCLUDE FILE
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 24635 $
 **
 ***************************************************************************/
#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <intrinsics.h>

#include "stm32f10x_lib.h"

#include "arm_comm.h"

#include "drv_glcd_cnfg.h"
#include "drv_glcd.h"
#include "glcd_ll.h"


#define B1_MASK         GPIO_Pin_8
#define B1_PORT         GPIOG

#define B2_MASK         GPIO_Pin_0
#define B2_PORT         GPIOA


#endif /* __INCLUDES_H */
