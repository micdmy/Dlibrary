#include "DConfig.h"
#ifndef D_SPI_H
#define D_SPI_H

#define D_SPI_ID1 SPI1
#define D_SPI_ID1_IRQ_HANDLER SPI1_IRQHandler

#define D_SPI_BR_DIV_2		0
#define D_SPI_BR_DIV_4		1
#define D_SPI_BR_DIV_8		2
#define D_SPI_BR_DIV_16		3
#define D_SPI_BR_DIV_32		4
#define D_SPI_BR_DIV_64		5
#define D_SPI_BR_DIV_128	6
#define D_SPI_BR_DIV_256	7
#define D_SPI_CLK_POL_IDLE_LOW 0
#define D_SPI_CLK_POL_IDLE_HIGH 1
#define D_SPI_CLK_PHA_MIDDLE_OF_BIT 0
#define D_SPI_CLK_PHA_START_OF_BIT 1
#define D_SPI_ENDIANNES_LSB_FIRST 1
#define D_SPI_ENDIANNES_MSB_FIRST 0
#define D_SPI_MODE_MASTER 1
#define D_SPI_MODE_SLAVE 0
#define D_SPI_NSS_SOFTWARE 1
#define D_SPI_NSS_HARDWARE 0
#define D_SPI_NSS_OUT_ENABLE 1
#define D_SPI_NSS_OUT_DISABLE 0
#define D_SPI_INTERRUPT_TX 		1ul << 7
#define D_SPI_INTERRUPT_RX 		1ul << 6
#define D_SPI_INTERRUPT_ERROR   1ul << 5
#define D_SPI_INTERRUPTS_NONE	0
typedef struct {
	uint8_t baudRateDiv;
	uint8_t clockPolarity;
	uint8_t clockPhase;
	uint8_t endiannes;
	uint8_t mode;
	uint8_t NSSType;
	uint8_t NSSOut;
	uint8_t interrupts;
	uint8_t interruptPriority;


}DSpiConfiguration;

void DSpiConfig(SPI_TypeDef * spi, DSpiConfiguration * dSpiConfiguration);

void DSpiSend(SPI_TypeDef * spi, uint8_t data);

uint32_t DSpiDataAvailable(SPI_TypeDef * spi);

uint16_t DSpiGetReceivedData(SPI_TypeDef * spi);


#endif //D_SPI_H
