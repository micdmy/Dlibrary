#ifndef D_L4USART_H
#define D_L4USART_H

#define USART_REC_L 16
extern int8_t usartRec[USART_REC_L];
extern volatile uint8_t recNum;

#define MY_USART USART2
#define MY_USART_INTERRUPT_HANDLER USART2_IRQHandler

void DusartConfig(void);

void DusartStopReceiveViaInterrupts(void);

void DusartResumeReceiveViaInterrupts(void);

#endif //D_L4USART_H
