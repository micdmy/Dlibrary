#ifndef D_L4USART_H
#define D_L4USART_H

#define USART_REC_L 16
extern int8_t usartRec[USART_REC_L];
extern volatile uint8_t recNum;

#define MY_USART USART2
#define MY_USART_INTERRUPT_HANDLER USART2_IRQHandler

extern void DusartConfig(void);

extern void DusartStopReceiveViaInterrupts(void);

extern void DusartResumeReceiveViaInterrupts(void);

extern uint32_t DusartSendViaInterrupts(uint8_t * data, uint32_t size);

extern void DusartSendViaDma(uint8_t * dataToSend, uint32_t length);

#endif //D_L4USART_H
