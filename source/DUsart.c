#include "DConfig.h"
#ifdef D_COMPONENT_USART

volatile uint32_t sendCount; //counter of already sent data bytes, zeroed in DUsartSendViaInterrupts(), incremented in DUsartSendingComplete()
volatile uint32_t dataSize;  //number of data bytes to send via interrupt, set in DUsartSendViaInterrupts(), used in DUsartSendingComplete()
volatile uint8_t * dataPtr;	///pointer to first byte of data to send, set in DUsartSendViaInterrupts(), used in DUsartSendingComplete()

void DUsartConfig(USART_TypeDef * usart, DUsartConfiguration * dUsartConfiguration) {
	if(usart == D_USART_ID1){																		//configuration of D_USART_ID1
		RCC->CCIPR |= dUsartConfiguration->clockSource << D_USART_ID1_CLK_SOURCE_OFFSET; 			//choose clock source for D_USART_ID1
		RCC->D_USART_ID1_CLK_ENABLE_RCC_REG |= D_USART_ID1_CLK_ENABLE_BIT_MSK;  					//enable clock for D_USART_ID1
		if(dUsartConfiguration->interruptOption) {													//using D_USART_ID1 with interrupts
			NVIC_EnableIRQ(D_USART_ID1_IRQ);														//enable interrupts for D_USART_ID1
			NVIC_SetPriority(D_USART_ID1_IRQ, dUsartConfiguration->NVICPriority);											//set priority of interrupts for D_USART_ID1
		}
	} else if(usart == D_USART_ID2) {																//configuration of D_USART_ID2
		RCC->CCIPR |= dUsartConfiguration->clockSource << D_USART_ID2_CLK_SOURCE_OFFSET;            //choose clock source for D_USART_ID2
		RCC->D_USART_ID2_CLK_ENABLE_RCC_REG |= D_USART_ID2_CLK_ENABLE_BIT_MSK;  					//enable clock for D_USART_ID2
		if(dUsartConfiguration->interruptOption) {                                                  //using D_USART_ID2 with interrupts
			NVIC_EnableIRQ(D_USART_ID2_IRQ);                                    					//enable interrupts for D_USART_ID2
			NVIC_SetPriority(D_USART_ID2_IRQ, dUsartConfiguration->NVICPriority);                   //set priority of interrupts for D_USART_ID2
		}
	}
	if(dUsartConfiguration->matchedCharacter & D_USART_MATCHED_CHARACTER(0x00)) {					//user wants to match character
		usart->CR2 |= ((uint32_t)dUsartConfiguration->matchedCharacter) << 24;						//save match character in CR2 register
	}
	usart->BRR = dUsartConfiguration->BBRValue; 													//set baud rate register value for usart
	usart->CR1 = (dUsartConfiguration->interruptOption & D_USART_INTERRUPT_OPTION_RX) |				//enable "byte received" interrupt, if interruptOption != D_USART_INTERRUPT_OPTION_DISABLED
				 (dUsartConfiguration->matchedCharacter & USART_CR1_CMIE) |							//enable "character matched" interrupt, if character was defined by user in DUsartConfiguration
				 USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;										//enable usart, transmission, reception, and

}

uint32_t DUsartCharacterMatched(USART_TypeDef * usart) {
	if(usart->ISR & USART_ISR_CMF) { 	//character match interrupt flag set
		usart->ICR |= USART_ICR_CMCF | USART_ICR_ORECF; 	//reset character match interrupt flag
		usart->RQR |= USART_RQR_RXFRQ;  //flush receive register and clear RXNE flag
		usart->CR1 |= USART_CR1_RXNEIE; //enable data received interrupt
		return 1;
	} else {
		return 0;
	}

	//maybe it could be done faster this way, becouse USART_ISR_CMF == USART_ICR_CMCF:
	//return usart->ICR |= (usart->ISR & USART_ISR_CMF)
}

void DUsartSend(uint8_t data, USART_TypeDef * usart) {
	usart->TDR = data; 						//put data byte to transmit data register
	while(!(usart->ISR & USART_ISR_TC)); 	//wait for transmission to complete
}

uint32_t DUsartSendViaInterrupts(uint8_t * data, uint32_t size, USART_TypeDef * usart) {
	if(usart->CR1 & USART_CR1_TCIE) {		//there is ongoing transfer
		return 0;							//unable to initialize sending
	} else {
		dataSize = size;					//number of data bytes to send via interrupt
		dataPtr = data;						//pointer to first byte of data to send
		sendCount = 0;						//zero the counter of already sent data bytes
		usart->CR1 |= USART_CR1_TCIE; 		//enable "transmission of a byte completed" interrupt
		return 1;							//sending initialized successfully
	}

}

uint32_t DUsartSendingComplete(USART_TypeDef * usart) {
	if( (usart->ISR & USART_ISR_TC)  ) { 		//transmission of a byte completed
		if(sendCount < dataSize) { 					//there is next data byte to send
			usart->TDR = dataPtr[sendCount++];	//put data byte to transmit data register
		} else {									//no more data bytes to send
			usart->CR1 &= ~USART_CR1_TCIE;		//disable "transmission of a byte completed" interrupt
			return 1; 								//1 = all bytes sent
		}
	}
	return 0; 										//0 = there are still some bytes to send
}

uint32_t DUsartDataAvailable(USART_TypeDef * usart) {
	return usart->ISR & USART_ISR_RXNE;	//returns 0 when no bytes received, else returns true (0x00000020)
}

uint32_t DUsartGetReceivedData(USART_TypeDef * usart) {
	return usart->RDR; //returns received data byte
}

void DUsartStopReceiveViaInterrupts(USART_TypeDef * usart) {
	usart->CR1 &= ~ USART_CR1_RXNEIE;
}
void DUsartResumeReceiveViaInterrupts(USART_TypeDef * usart) {
	usart->RQR |= USART_RQR_RXFRQ;
	usart->ICR |= USART_ICR_ORECF;
	usart->CR1 |= USART_CR1_RXNEIE;
}

#endif //D_COMPONENT_USART
