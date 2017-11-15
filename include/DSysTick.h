#ifndef D_SYSTICK_H
#define D_SYSTICK_H

/*
 * Used components. Comment to disable or uncomment to enable.
 */
//#define D_SYSTICK_COMPONENT_POLLING
#define D_SYSTICK_COMPONENT_POLLING_DELAY
//#define D_SYSTICK_COMPONENT_INTERRUPT
/*
 * Options. Comment to disable or uncomment to enable.
 */
//#define D_SYSTICK_OPTION_EXTERNAL_CLK


/* ================================
 * Below is code. Not for user use.
   ================================*/

#ifdef D_SYSTICK_COMPONENT_POLLING
	/*
	 * Configures SysTick for polling.
	 */
	void DSysTickConfig(uint32_t ticksPeriod);
	/*
	 * Checks if SysTick has counted to 0, if true, stops SysTick.
	 */
	uint32_t DSysTickEnd();
#endif //D_SYSTICK_COMPONENT_POLLING

#ifdef D_SYSTICK_COMPONENT_POLLING_DELAY
	void DSysTickDelay(uint32_t ticksTime);
#endif //D_SYSTICK_COMPONENT_POLLING_DELAY

#ifdef D_SYSTICK_COMPONENT_INTERRUPT
	/*
	 * Configures SysTick for use with interrupt handler.
	 * To use least important priority use: (1<<__NVIC_PRIO_BITS) - 1
	 * Handler function for ISR: SysTick_Handler(void)
	 * Example:
	 * DSysTickConfig(SystemCoreClock/[USER_FREQUENCY], (1<<__NVIC_PRIO_BITS) - 1);
	 */
	void DSysTickConfig(uint32_t ticksPeriod, uint32_t NVICPriority);
#endif //D_SYSTICK_COMPONENT_INTERRUPT

#ifdef D_SYSTICK_OPTION_EXTERNAL_CLK
	#define D_SYSTICK_CLK_SOURCE 0
#else
	#define D_SYSTICK_CLK_SOURCE SysTick_CTRL_CLKSOURCE_Msk
#endif //D_SYSTICK_OPTION_EXTERNAL_CLK

#endif //D_SYSTICK_H
