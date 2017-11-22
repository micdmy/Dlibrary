#include "DConfig.h"
#ifdef D_COMPONENT_L4USART


int8_t usartRec[USART_REC_L]={0};
volatile uint8_t recNum = 0;

inline static void _enablePins(void);
inline static void _enableClks(void);

void DusartConfig(void) {

	_enableClks();
	MY_USART->BRR = 0x567; //57,6baud on fck =80MHz
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 2);
	MY_USART->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_RXNEIE;
	_enablePins();
}

void DusartStopReceiveViaInterrupts(void) {
	MY_USART->CR1 &= ~ USART_CR1_RXNEIE;
}
void DusartResumeReceiveViaInterrupts(void) {
	MY_USART->RQR |= USART_RQR_RXFRQ;
	MY_USART->ICR |= USART_ICR_ORECF;
	MY_USART->CR1 |= USART_CR1_RXNEIE;
}

void MY_USART_INTERRUPT_HANDLER(void) {
	if(MY_USART->ISR & USART_ISR_RXNE) {
		usartRec[recNum] = MY_USART->RDR;
		recNum = (recNum+1) & (USART_REC_L-1); //circle buffer
	}
}

inline static void _enablePins(void) {
	/*
	 * TX - PA2
	 * RX - PA15
	 */
	RCC->NUCLEO_USART_STLINK_RCC_CLK_REG |= NUCLEO_USART_STLINK_RCC_CLK_BIT;
	#define PORT NUCLEO_USART_STLINK_PORT_GPIOA
	PORT->MODER &=~(NUCLEO_USART_STLINK_MODE_ZERO);
	PORT->MODER |= NUCLEO_USART_STLINK_MODE_ALT;
	PORT->NUCLEO_USART_STLINK_TX_ALT_FUNC_REG |= NUCLEO_USART_STLINK_TX_ALT_FUNC_BIT_MASK;
	PORT->NUCLEO_USART_STLINK_RX_ALT_FUNC_REG |= NUCLEO_USART_STLINK_RX_ALT_FUNC_BIT_MASK;
}

inline static void _enableClks(void) {
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0; //SysClk Clock source
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
}
#endif
