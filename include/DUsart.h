#ifndef D_USART_H
#define D_USART_H


/* ================================
 * Below is code. Not for user use.
   ================================*/

#define D_USART_ID1 USART1 //connected to: RX-PA10,PB7 ; TX-PA9,PB6 (SMT32L053)
#define D_USART_ID2 USART2 // connectted to STLink -for now library can use onlu this usart

#define D_USART_ID1_IRQ_HANDLER USART1_IRQHandler
#define D_USART_ID2_IRQ_HANDLER USART2_IRQHandler

#define D_USART_ID1_CLK_SOURCE_OFFSET 0x00
#define D_USART_ID1_CLK_ENABLE_RCC_REG APB2ENR
#define D_USART_ID1_CLK_ENABLE_BIT_MSK RCC_APB2ENR_USART1EN
#define D_USART_ID1_IRQ USART1_IRQn

#define D_USART_ID2_CLK_SOURCE_OFFSET 0x02
#define D_USART_ID2_CLK_ENABLE_RCC_REG APB1ENR
#define D_USART_ID2_CLK_ENABLE_BIT_MSK RCC_APB1ENR_USART2EN
#define D_USART_ID2_IRQ USART2_IRQn

#define D_USART_CLK_SOURCE_APB 			0x00
#define	D_USART_CLK_SOURCE_SYSTEM_CLK 	0x01
#define D_USART_CLK_SOURCE_HSI16 		0x02
#define D_USART_CLK_SOURCE_LSE 			0x03

#define D_USART_INTERRUPT_OPTION_DISABLED 	0x00
#define D_USART_INTERRUPT_OPTION_RX 		USART_CR1_RXNEIE
#define D_USART_INTERRUPT_OPTION_TX 		0x01
#define D_USART_INTERRUPT_OPTION_RX_TX 		D_USART_INTERRUPT_OPTION_RX

#define D_USART_MATCHED_CHARACTER_NONE 		0
#define D_USART_MATCHED_CHARACTER(matched_character) USART_CR1_CMIE | matched_character

typedef struct {
	uint8_t	 clockSource;
	uint8_t	 interruptOption;
	uint16_t matchedCharacter;
	uint32_t BBRValue;
	uint32_t NVICPriority;
}DUsartConfiguration;

/*
 * Configures chosen usart and initializes its work.
 * Parameters:
 * usart =
 * 	D_USART_ID1
 * 	D_USART_ID2
 * BBRValue =
 * 	(frequency of usart source clock) / (baudrate)
 * clockSource =
 * 	D_USART_CLK_SOURCE_APB
 * 	D_USART_CLK_SOURCE_SYSTEM_CLK
 * 	D_USART_CLK_SOURCE_HSI16
 * 	D_USART_CLK_SOURCE_LSE
 * interruptOption =
 * 	D_USART_INTERRUPT_OPTION_DISABLED
 * 	D_USART_INTERRUPS_OPTION_RX
 * 	D_USART_INTERRUPS_OPTION_TX
 * 	D_USART_INTERRUPS_OPTION_RX_TX
 * NVICPriority = number from 0 (highest) to (1<<__NVIC_PRIO_BITS) - 1 (lowest)
 */
void DUsartConfig(USART_TypeDef * usart, DUsartConfiguration * dUsartConfiguration);

uint32_t DUsartCharacterMatched(USART_TypeDef * usart);

/*
 * Sends one byte and waits until transmission completed.
 * Use it for sending single byte without using interrupts.
 */
void DUsartSend(uint8_t data, USART_TypeDef * usart);

/*
 * Sends array of uint8_t data of given size.
 * This function only enables transmit complete interrupts, in which DUsartSendingComplete() sends data.
 * Returns 1 if sending initialized successfully.
 * Returns 0 if sending is already pending, and can't initialize a new one.
 */
uint32_t DUsartSendViaInterrupts(uint8_t * data, uint32_t size, USART_TypeDef * usart);

/*
 * Should be called in USARTx_IRQHandler() interrupt handler.
 * Sends next data byte if there are still bytes to send.
 * Returns true, if all bytes sent.
 */
uint32_t DUsartSendingComplete(USART_TypeDef * usart);

/*
 * Can be called in main program or in USARTx_IRQHandler() interrupt handler.
 * Returns true, if byte has been received.
 */
uint32_t DUsartDataAvailable(USART_TypeDef * usart);

/*
 * Returns received data byte.
 * Should be called immediately after DUsartSendingComplete() returned true.
 */
uint32_t DUsartGetReceivedData(USART_TypeDef * usart);

/*
 * Disables receive interrupt.
 */
void DUsartStopReceiveViaInterrupts(USART_TypeDef * usart);

/*
 * Clears reception buffer.
 * Clears overrun and reception flags.
 * Then, enables receive interrupt.
 */
void DUsartResumeReceiveViaInterrupts(USART_TypeDef * usart);

#endif //D_USART_H
