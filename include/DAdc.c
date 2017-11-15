#include "adc.h"

inline static void _enableClks(void);
inline static void _enableInputPin(void);
inline static void _setPrescallerAndClkSrc(void);
inline static void _enable(void);

_enableVoltReg(void)

/*
 * Sets register values in ADC peripheral.
 */
void adcConfig(void) {
	_adcEnableInputPin();
	_adcEnableClks();
	_setPrescallerAndClkSrc();

}

inline static void _enableClks(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; //chose AHB clock for ADCs
	RCC->CCIPR |= RCC_CCIPR_ADCSEL; // SysClk choosen for ADCs
}

inline static void _enableInputPin(void) {
	//TODO:
	//implementation
}

inline static void _setPrescallerAndClkSrc(void) {
	MY_ADC->CCR = ADC_CCR_CKMODE_0; // prescaler = 1, HCLK/1 as source clock
}

inline static void _enable(void) {
	MY_ADC->ISR |= ADC_ISR_ADRDY; //zero ADRDY flag
	MY_ADC->CR	//enable ADC
	while(!(MY_ADC->ISR & ADC_ISR_ADRDY)); //wait until ADC ready
}

inline static void _deepPowerDownEnable(void) {
	MY_ADC->CR |= ADC_CR_DEEPPWD;
}

inline static void _deepPowerDownEnable(void) {
	MY_ADC->CR &= ~(ADC_CR_DEEPPWD);
}

inline static void _enableVoltReg(void) {
	MY_ADC->CR |= ADC_CR_ADVREGEN;
}

inline static void _disableVoltReg(void) {
	MY_ADC->CR &= ~(ADC_CR_ADVREGEN);
}

inline static void _enableVoltReg(void) {
	MY_ADC->CR |= ADC_CR_ADVREGEN;
	//wait 20us -implement by systick
}
