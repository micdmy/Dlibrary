/*
 * This is custom stm32 library written by Michał Dmytruszyński.
 * Only this file should be included in main.c.
 * Below one can choose, which components to use
 */
#ifndef DLIBRARY
#define DLIBRARY

#define D_SYSCLK_FREQ 80000000
/*
 * Used components. Comment to disable or uncomment to enable.
 */
#define D_COMPONENT_SYSTICK
//#define D_COMPONENT_USART
//#define D_COMPONENT_SPI
#define D_COMPONENT_ADC
/* ================================
 * Below is code. Not for user use.
   ================================*/
#include "stdint.h"
//#include <stm32L053xx.h>
//#include "stm32f10x.h"
#include "stm32l432xx.h"

#ifdef D_COMPONENT_SYSTICK

	#include "DSysTick.h"
#endif
#ifdef D_COMPONENT_USART
	#include "DUsart.h"
#endif
#ifdef D_COMPONENT_SPI
	#include "DSpi.h"
#endif
#ifdef D_COMPONENT_ADC
	#include "DAdc.h"
#endif


#endif //DLIBRARY
