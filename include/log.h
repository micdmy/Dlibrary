#ifndef LOG_H
#define LOG_H


#define LOG_SIZE 32


typedef enum {
 EMPTY,
 DMA_int_error,
 DMA_int_tComplete,
 DMA_int_unexpError,
 DMA_start,
 DMA_USART_int_error,
 DMA_USART_int_tComplete,
 DMA_USART_int_unexpError,
 DMA_USART_start,
 ADC_start,
 ADC_int_ovr,
 ADC_int_unexpError,
 ADC_stopCommand,
 USART_rx_int,
 USART_tx_int
}logEntry;

extern logEntry logBuffer[LOG_SIZE];
extern volatile uint8_t logLast;

extern void logReset (void);

extern void logAdd(logEntry newEntry);

#endif //LOG_H
