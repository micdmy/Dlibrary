#include "DConfig.h"
#ifdef D_COMPONENT_ADC

inline static void _enableClks(void);
inline static void _enableInputPin(void);
inline static void _setPrescallerAndClkSrc(void);
inline static void _enable(void);
inline static void _deepPowerDownEnable(void);
inline static void _deepPowerDownDisable(void);
inline static void _disableVoltReg(void);
inline static void _enableVoltReg(void);
inline static void _calibrate(void);
inline static void _resetConfig(void);
inline static void _chooseContinuousConverion(void);
inline static void _chooseDma(void);
inline static void _oneRegularConversionOnIn5(void);
inline static void _chooseSamplingTime(void);

/*
 * Sets register values in ADC peripheral.
 * Prepares ADC to work choose.
 */
void adcConfig(void) {
	_enableInputPin();
	_enableClks();
	_resetConfig();
	_setPrescallerAndClkSrc();
	_deepPowerDownDisable();
	MY_ADC->CR |= ADC_CR_ADVREGEN;
	DSysTickDelay(D_SYSCLK_FREQ / 5); //wait 20us, for generator init 200ms
	MY_ADC->CR |= ADC_CR_ADCAL;
	while(!(MY_ADC->CR & ADC_CR_ADCAL));
	//MY_ADC->ISR |= ADC_ISR_ADRDY; //zero ADRDY flag
	MY_ADC->CR |= ADC_CR_ADEN; //enable ADC
	while(!(MY_ADC->ISR & ADC_ISR_ADRDY)); //wait until ADC ready
	//_enableVoltReg();
	//_calibrate();
	//_enable();

	//_chooseContinuousConverion();
	MY_ADC->CFGR &=~(ADC_CFGR_CONT); //single conversion
	//_chooseDma();
	_oneRegularConversionOnIn5();
	_chooseSamplingTime();
}

void adcStart(void) {
	MY_ADC->CR |= ADC_CR_ADSTART;
	logAdd(ADC_start);
}

void MY_ADC_INTERRUPT_HANDLER(void) {
	if(MY_ADC->ISR & ADC_ISR_OVR) { //ADC overrun error flag
		logAdd(ADC_int_ovr);
	} else { //unexpected flag, should not occur
		logAdd(ADC_int_ovr);
	}
}

void adcStop(void) {
	MY_ADC->CR |= ADC_CR_ADSTP;
	//to ensure that ADC has stopped, check if ADC_CR_ADSTART == 0
	logAdd(ADC_stopCommand);
}

inline static void _enableClks(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; //chose AHB clock for ADCs
	RCC->CCIPR |= RCC_CCIPR_ADCSEL; // SysClk choosen for ADCs
}

inline static void _enableInputPin(void) {
	RCC->NUCLEO_ADC1_IN5_RCC_CLK_REG |= NUCLEO_ADC1_IN5_RCC_CLK_BIT;
	NUCLEO_ADC1_IN5_PORT_GPIOA->MODER |= NUCLEO_ADC1_IN5_PIN_MODE_ANALOG;
}

inline static void _setPrescallerAndClkSrc(void) {
	//MY_ADC_COMMON->CCR = ADC_CCR_CKMODE_0; // prescaler = 1, HCLK/1 as source clock
	MY_ADC_COMMON->CCR = ADC_CCR_CKMODE_0 | ADC_CCR_PRESC_0 | ADC_CCR_PRESC_3; //prescaler =64
}

inline static void _enable(void) {
	//MY_ADC->ISR |= ADC_ISR_ADRDY; //zero ADRDY flag
	MY_ADC->CR |= ADC_CR_ADEN; //enable ADC
	while(!(MY_ADC->ISR & ADC_ISR_ADRDY)); //wait until ADC ready
}

inline static void _deepPowerDownEnable(void) {
	MY_ADC->CR |= ADC_CR_DEEPPWD;
}

inline static void _deepPowerDownDisable(void) {
	MY_ADC->CR &= ~(ADC_CR_DEEPPWD);
}

inline static void _disableVoltReg(void) {
	MY_ADC->CR &= ~(ADC_CR_ADVREGEN);
}

inline static void _enableVoltReg(void) {

}

inline static void _calibrate(void) {

}

inline static void _resetConfig() {
	MY_ADC->CR = 0x20000000;
	MY_ADC->IER = 0x00000000;
	MY_ADC->CFGR = 0x80000000;
	MY_ADC->CFGR2 = 0x80000000;
	MY_ADC_COMMON->CCR = 0x00000000;
}

inline static void _chooseContinuousConverion(void) {
	MY_ADC->CFGR |= ADC_CFGR_CONT;
}

inline static void _chooseDma(void) {
	MY_ADC->CFGR |= ADC_CFGR_DMAEN;
}

inline static void _regularChannelSequenceLength(uint8_t length_from1_to16) {
	MY_ADC->SQR1 &= ADC_SQR1_L; //zero sequence length bit field
	MY_ADC->SQR1 |= ADC_SQR1_L & (length_from1_to16 - 1);
}

inline static void _oneRegularConversionOnIn5(void) {
#define field_length 6
#define channel 5
#define sequence 1
#define nr_of_sequences 1
	MY_ADC->SQR1 = (ADC_SQR1_SQ1 & (channel << (field_length*sequence))) | (ADC_SQR1_L & nr_of_sequences); //convert only channel number 5
}

inline static void _chooseSamplingTime(void) {
#define sampl_cycles_2_5   0
#define sampl_cycles_6_5   1
#define sampl_cycles_12_5  2
#define sampl_cycles_24_5  3
#define sampl_cycles_47_5  4
#define sampl_cycles_92_5  5
#define sampl_cycles_247_5 6
#define sampl_cycles_640_5 7
#define channel 5
	MY_ADC->SMPR1 = sampl_cycles_2_5 << (3*channel);
}
#endif //D_COMPONENT_ADC
