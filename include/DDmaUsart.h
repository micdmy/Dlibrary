#ifndef D_DMA_USART_H
#define D_DMA_USART_H

#define RCC_CLK_BIT_USART_DMA RCC_AHB1ENR_DMA1EN
#define USART_DMA DMA1
#define USART_TX_DMA_CH DMA1_Channel7
#define USART_DMA_CSELR DMA1_CSELR
#define USART_TX_DMA_INTERRUPT_HANDLER DMA1_Channel7_IRQHandler
#define USART_TX_DMA_CH_TCIF DMA_ISR_TCIF7
#define USART_TX_DMA_CH_HTIF DMA_ISR_HTIF7
#define USART_TX_DMA_CH_TEIF DMA_ISR_TEIF7

extern void dmaForUsartConfig(void);

extern void dmaUsartStartTX(uint8_t * memoryBuffer, uint32_t bufferSize);

#endif //D_DMA_USART_H
