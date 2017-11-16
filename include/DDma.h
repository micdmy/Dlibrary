#ifndef D_DMA_H
#define D_DMA_H

#define RCC_CLK_BIT_MY_DMA RCC_AHB1ENR_DMA1EN
#define MY_DMA DMA1
#define MY_DMA_CH DMA1_Channel1
#define MY_DMA_CSELR DMA1_CSELR
#define DMA_FOR_ADC_INTERRUPT_HANDLER DMA1_Channel1_IRQHandler
#define MY_DMA_CH_TCIF DMA_ISR_TCIF1
#define MY_DMA_CH_HTIF DMA_ISR_HTIF1
#define MY_DMA_CH_TEIF DMA_ISR_TEIF1

extern void dmaForAdcConfig(volatile uint8_t * memoryBuffer, uint32_t bufferSizeIn16b);

extern void dmaForAdcStart(void);

#endif //D_DMA_H
