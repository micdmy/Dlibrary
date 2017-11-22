#include "DConfig.h"
#ifdef D_COMPONENT_L4USART


int8_t usartRec[USART_REC_L]={0};
volatile uint8_t recNum = 0;

volatile uint32_t sendCount; //counter of already sent data bytes, zeroed in DUsartSendViaInterrupts(), incremented in DUsartSendingComplete()
volatile uint32_t dataSize;  //number of data bytes to send via interrupt, set in DUsartSendViaInterrupts(), used in DUsartSendingComplete()
volatile uint8_t * dataPtr;	///pointer to first byte of data to send, set in DUsartSendViaInterrupts(), used in DUsartSendingComplete()



inline static void _enablePins(void);
inline static void _enableClks(void);

void DusartConfig(void) {

	_enableClks();
	MY_USART->BRR = 0x567; //57,6baud on fck =80MHz
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 2);
	MY_USART->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
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

uint32_t DusartSendViaInterrupts(uint8_t * data, uint32_t size) {
	if(MY_USART->CR1 & USART_CR1_TCIE) {		//there is ongoing transfer
		return 0;							//unable to initialize sending
	} else {
		dataSize = size;					//number of data bytes to send via interrupt
		dataPtr = data;						//pointer to first byte of data to send
		sendCount = 0;						//zero the counter of already sent data bytes
		MY_USART->CR1 |= USART_CR1_TCIE; 		//enable "transmission of a byte completed" interrupt
		return 1;							//sending initialized successfully
	}
	return 0; //to mute warning
}

void MY_USART_INTERRUPT_HANDLER(void) {
	if(MY_USART->ISR & USART_ISR_RXNE) {
		usartRec[recNum] = MY_USART->RDR;
		recNum = (recNum+1) & (USART_REC_L-1); //circle buffer
	} else
	if( (MY_USART->ISR & USART_ISR_TC)  ) { 		//transmission of a byte completed
		if(sendCount < dataSize) { 					//there is next data byte to send
			MY_USART->TDR = dataPtr[sendCount++];	//put data byte to transmit data register
		} else {									//no more data bytes to send
			MY_USART->CR1 &= ~(USART_CR1_TCIE);		//disable "transmission of a byte completed" interrupt
		}
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
