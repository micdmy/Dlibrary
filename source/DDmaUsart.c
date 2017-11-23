#include "DConfig.h"
#ifdef D_COMPONENT_DMA_USART

void dmaForUsartConfig(void) {
	RCC->AHB1ENR |= RCC_CLK_BIT_USART_DMA; //enable clock for DMA
	USART_TX_DMA_CH->CCR =
			//	DMA_CCR_MEM2MEM | //no memory to memory
				DMA_CCR_PL | //very high priority channel
			//	DMA_CCR_MSIZE | //8b size, zeroed
			//	DMA_CCR_PSIZE | //8b size, zeroed
				DMA_CCR_MINC | //memory increment
			//	DMA_CCR_PINC | //no periph increment
			//	DMA_CCR_CIRC | //no circular mode
				DMA_CCR_DIR  | //read from memory
				DMA_CCR_TEIE | //transfer error interrupt
			//	DMA_CCR_HTIE | //no half-transfer interrupt
				DMA_CCR_TCIE ; //transfer complete interrupt
	USART_TX_DMA_CH->CPAR = (uint32_t) (&(USART2->TDR));
	USART_DMA_CSELR->CSELR = 2<<24; //channel 7 mapped on USART2_TX
	USART_DMA->IFCR = 0x00FFFFFFF; //clear interrupts flags
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	NVIC_SetPriority(DMA1_Channel7_IRQn,0);
}

void dmaUsartStartTX(uint8_t * memoryBuffer, uint32_t bufferSize) {
	/*
	 * DMA channel should be disabled at this point.
	 */
	USART_TX_DMA_CH->CMAR = (uint32_t) memoryBuffer;
	USART_TX_DMA_CH->CNDTR = bufferSize;
	USART_TX_DMA_CH->CCR |= DMA_CCR_EN; //enable DMA channel
	logAdd(DMA_USART_start);
}

void USART_TX_DMA_INTERRUPT_HANDLER(void) {
	if(USART_DMA->ISR & USART_TX_DMA_CH_TEIF) { // DMA transfer error
		logAdd(DMA_USART_int_error);
		USART_DMA->IFCR |= USART_TX_DMA_CH_TEIF; //clear flag
	} else if(MY_DMA->ISR & USART_TX_DMA_CH_TCIF) { //DMA transfer completed
		logAdd(DMA_USART_int_tComplete);
		USART_TX_DMA_CH->CCR &=~(DMA_CCR_EN); //disable DMA channel
		USART_DMA->IFCR |= USART_TX_DMA_CH_TCIF; //clear flag
	} else { //unexpected behavior, should never occur, if DMA_CCR_HTIE disabled
		logAdd(DMA_USART_int_unexpError);
	}
}


#endif //D_COMPONENT_DMA_USART
