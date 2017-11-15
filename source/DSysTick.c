#include "DConfig.h"
#ifdef D_COMPONENT_SYSTICK


#ifdef D_SYSTICK_COMPONENT_POLLING
	void DSysTickConfig(uint32_t ticksPeriod) {
		SysTick->LOAD = ticksPeriod - 1; //set reload register
		SysTick->VAL   = 0; //any write here sets initial value of counter to 0
		SysTick->CTRL  = D_SYSTICK_CLK_SOURCE | //processor clock as source or external
						 SysTick_CTRL_ENABLE_Msk; //enable SysTick
	}

	uint32_t DSysTickEnd() {
		if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			SysTick->CTRL = 0;
			return 1;
		} else {
			return 0;
		}
	}
#endif //D_SYSTICK_COMPONENT_POLLING
#ifdef D_SYSTICK_COMPONENT_POLLING_DELAY
	void DSysTickDelay(uint32_t ticksTime) {
		SysTick->LOAD = ticksTime - 1; //set reload register
		SysTick->VAL   = 0; //any write here sets initial value of counter to 0
		SysTick->CTRL  = D_SYSTICK_CLK_SOURCE | //processor clock as source or external
					     SysTick_CTRL_ENABLE_Msk; //enable SysTick
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); //wait until SysTick ends counting
	}
#endif //D_SYSTICK_COMPONENT_POLLING_DELAY
#ifdef D_SYSTICK_COMPONENT_INTERRUPT
	void DSysTickConfig(uint32_t ticksPeriod, uint32_t NVICPriority) {
		NVIC_SetPriority (SysTick_IRQn, NVICPriority); //set priority of interrupt
		SysTick->LOAD = ticksPeriod - 1; //set reload register
		SysTick->VAL   = 0; //any write here sets initial value of counter to 0
		SysTick->CTRL  = D_SYSTICK_CLK_SOURCE | //processor clock as source or external
						SysTick_CTRL_TICKINT_Msk | //enable interrupt
						 SysTick_CTRL_ENABLE_Msk; //enable SysTick
	}
#endif //D_SYSTICK_COMPONENT_INTERRUPT

#endif //COMPONENT_SYSTICK
