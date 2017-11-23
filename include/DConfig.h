/*
 * This is custom stm32 library written by Micha³ Dmytruszyñski.
 * Only this file should be included in main.c.
 * Below one can choose, which components to use
 */
#ifndef DLIBRARY
#define DLIBRARY

#define D_SYSCLK_FREQ 80000000
/*
 * Used components. Comment to disable or uncomment to enable.
 */
#define D_COMPONENT_LOG
#define D_COMPONENT_SYSTICK
//#define D_COMPONENT_USART
#define D_COMPONENT_L4USART
//#define D_COMPONENT_SPI
#define D_COMPONENT_ADC
#define D_COMPONENT_DMA
#define D_COMPONENT_DMA_USART
/* ================================
 * Below is code. Not for user use.
   ================================*/
#include "stdint.h"
//#include <stm32L053xx.h>
//#include "stm32f10x.h"
#include "stm32l432xx.h"

#include "DNucleoL432KC.h"

#ifdef D_COMPONENT_SYSTICK

	#include "DSysTick.h"
#endif
#ifdef D_COMPONENT_USART
	#include "DUsart.h"
#endif
#ifdef D_COMPONENT_L4USART
#include "DL4Usart.h"
#endif
#ifdef D_COMPONENT_SPI
	#include "DSpi.h"
#endif
#ifdef D_COMPONENT_ADC
	#include "DAdc.h"
#endif
#ifdef D_COMPONENT_DMA
	#include "DDma.h"
#endif
#ifdef D_COMPONENT_LOG
	#include "log.h"
#endif
#ifdef D_COMPONENT_DMA_USART
	#include "DDmaUsart.h"
#endif
#endif //DLIBRARY
