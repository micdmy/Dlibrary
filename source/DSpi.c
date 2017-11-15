#include "DConfig.h"
#ifdef D_COMPONENT_SPI

#define D_SPI_ID1 SPI1
#define D_SPI_ID1_IRQ_HANDLER SPI1_IRQHandler

void DSpiConfig(SPI_TypeDef * spi, DSpiConfiguration * dSpiConfiguration) {
	if(spi == SPI1) {
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	spi->CR1 = ((dSpiConfiguration->baudRateDiv) << 3) |
			((dSpiConfiguration->clockPolarity) << 1) |
			((dSpiConfiguration->clockPhase) << 3) |
			((dSpiConfiguration->endiannes) << 7) |
			((dSpiConfiguration->mode) << 2) |
			((dSpiConfiguration->NSSType) << 9);
	spi->CR2 = ((dSpiConfiguration->NSSOut) << 2);
	spi->CR1 |= SPI_CR1_SSI; //usun potem, tylko gdy software NSS

	if(dSpiConfiguration->interrupts) {
		NVIC_EnableIRQ(SPI1_IRQn);
		NVIC_SetPriority(SPI1_IRQn,dSpiConfiguration->interruptPriority);
		spi->CR2 |= dSpiConfiguration->interrupts;
	}
	spi->CR1 |= SPI_CR1_SPE; //enable SPI
}

void DSpiSend(SPI_TypeDef * spi, uint8_t data) {
	*(uint8_t *)&spi->DR = data;	//put data to transmit buffer
	//spi->DR;
	spi->CR2 |= SPI_CR2_RXNEIE;
	while(!(spi->SR & SPI_SR_TXE)); //wait for transmission to complete
}

uint32_t DSpiDataAvailable(SPI_TypeDef * spi) {
	return spi->SR & SPI_SR_RXNE;
}
uint16_t DSpiGetReceivedData(SPI_TypeDef * spi) {
	return spi->DR;
}
#endif //D_COMPONENT_SPI
