/*
 * This is custom stm32 library written by Micha� Dmytruszy�ski.
 * Only this file should be included in main.c.
 * Below one can choose, which components to use
 */
#ifndef DLIBRARY
#define DLIBRARY
/*
 * Used components. Comment to disable or uncomment to enable.
 */
#define D_COMPONENT_SYSTICK
//#define D_COMPONENT_USART
//#define D_COMPONENT_SPI
/* ================================
 * Below is code. Not for user use.
   ================================*/
#include "stdint.h"
//#include <stm32L053xx.h>
#include "stm32f10x.h"

#ifdef D_COMPONENT_SYSTICK
	#include "DSysTick.h"
#endif
#ifdef D_COMPONENT_USART
	#include "DUsart.h"
#endif
#ifdef D_COMPONENT_SPI
	#include "DSpi.h"
#endif

#endif //DLIBRARY