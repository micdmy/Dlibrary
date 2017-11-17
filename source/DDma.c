#include "DConfig.h"
#ifdef D_COMPONENT_DMA

void dmaForAdcConfig(volatile uint8_t * memoryBuffer, uint32_t bufferSizeIn16b) {
	RCC->AHB1ENR |= RCC_CLK_BIT_MY_DMA; //enable clock for DMA
	MY_DMA_CH->CCR =
			//	DMA_CCR_MEM2MEM | //no memory to memory
				DMA_CCR_PL | //very high priority channel
				DMA_CCR_MSIZE_0 | //16b size
				DMA_CCR_PSIZE_0 | //16b size
				DMA_CCR_MINC | //memory increment
			//	DMA_CCR_PINC | //no periph increment
			//	DMA_CCR_CIRC | //no circular mode
			//	DMA_CCR_DIR  | //no read from memory
				DMA_CCR_TEIE | //transfer error interrupt
			//	DMA_CCR_HTIE | //no half-transfer interrupt
				DMA_CCR_TCIE ; //transfer complete interrupt
	MY_DMA_CH->CNDTR = bufferSizeIn16b;
	MY_DMA_CH->CPAR = (uint32_t) (&(ADC1->DR));
	MY_DMA_CH->CMAR = (uint32_t) memoryBuffer;
	MY_DMA_CSELR->CSELR = 0x00000000; //channel 1 mapped on ADC1
	MY_DMA->IFCR = 0x00FFFFFFF; //clear interrupts flags
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn,0);
}

void dmaForAdcStart(void) {
	MY_DMA_CH->CCR |= DMA_CCR_EN; //enable DMA channel
	logAdd(DMA_start);
}

void DMA_FOR_ADC_INTERRUPT_HANDLER(void) {
	if(MY_DMA->IFCR & MY_DMA_CH_TEIF) { // DMA transfer error
		logAdd(DMA_int_error);
	} else if(MY_DMA->IFCR & MY_DMA_CH_TEIF) { //DMA transfer completed
		logAdd(DMA_int_tComplete);
	} else { //unexpected behavior, should never occur, if DMA_CCR_HTIE disabled
		logAdd(DMA_int_unexpError);
	}
}
#endif //D_COMPONENT_DMA
