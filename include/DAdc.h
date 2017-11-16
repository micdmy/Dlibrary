#ifndef D_ADC_H
#define D_ADC_H

#define MY_ADC ADC1
#define MY_ADC_COMMON ADC1_COMMON
#define MY_ADC_INTERRUPT_HANDLER ADC1_IRQHandler

extern void adcConfig(void);

extern void adcStart(void);

/*
 * Hint:
 * In RCC ADC is mentioned in AHB2 registers and in CCIPR register.
 */

#endif //D_ADC_H
