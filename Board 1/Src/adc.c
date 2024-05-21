#include <adc.h>
#include <stm32f303xc.h>

ADCxPort ADC1_Port = {
		ADC1,
		ADC12_COMMON,
		RCC_AHBENR_ADC12EN,
		ADC12_CCR_CKMODE_0,
		ADC_CR_ADVREGEN,
		ADC_CR_ADVREGEN_0,
		ADC_CR_ADCALDIF,
		ADC_CR_ADCAL,
		0x02,
		ADC_SQR1_L_Pos,
		0x02,
		0x03,
		0x04,
		0x01,
		ADC_SQR1_SQ1_Pos,
		ADC_SQR1_SQ2_Pos,
		ADC_SQR1_SQ3_Pos,
		ADC_SQR1_SQ4_Pos,
		0x00,
		ADC_CR_ADEN,
		ADC_CR_ADSTART,
		0,	//The three values and three scales are all 0
		0,
		0,
		0,
		0, //Conversion counter = 0
		0,
		0,
		0,
		0,
};

ADCxPort ADC2_Port = {
		ADC2,
		ADC12_COMMON,
		RCC_AHBENR_ADC12EN,
		ADC12_CCR_CKMODE_0,
		ADC_CR_ADVREGEN,
		ADC_CR_ADVREGEN_0,
		ADC_CR_ADCALDIF,
		ADC_CR_ADCAL,
		0x02,
		ADC_SQR1_L_Pos,
		0x01,
		0x02,
		0x03,
		0x04,
		ADC_SQR1_SQ1_Pos,
		ADC_SQR1_SQ2_Pos,
		ADC_SQR1_SQ3_Pos,
		ADC_SQR1_SQ4_Pos,
		0x00, //Continuous or one shot mode
		ADC_CR_ADEN,
		ADC_CR_ADSTART,
		0,	//The three values and three scales are all 0
		0,
		0,
		0,
		0, //Conversion counter = 0
		0,
		0,
		0,
		0,
};

ADCxPort ADC3_Port = {
		ADC3,
		ADC34_COMMON,
		RCC_AHBENR_ADC34EN,
		ADC34_CCR_CKMODE_0,
		ADC_CR_ADVREGEN,
		ADC_CR_ADVREGEN_0,
		ADC_CR_ADCALDIF,
		ADC_CR_ADCAL,
		0x00,
		ADC_SQR1_L_Pos,
		0x01,
		0x02,
		0x03,
		0x04,
		ADC_SQR1_SQ1_Pos,
		ADC_SQR1_SQ2_Pos,
		ADC_SQR1_SQ3_Pos,
		ADC_SQR1_SQ4_Pos,
		0x00,
		ADC_CR_ADEN,
		ADC_CR_ADSTART,
		0,	//The four values and four scales are all 0
		0,
		0,
		0,
		0, //Conversion Counter
		0,
		0,
		0,
		0,
};


uint16_t ReadADC(uint32_t channel) {

	ADC1->SQR1 = 0;
	ADC1->SQR1 |= channel << ADC_SQR1_SQ1_Pos; // set the request for channel specified

	// request the process to start
	ADC1->CR |= ADC_CR_ADSTART;

	// Wait for the end of conversion
	while((ADC1->ISR &= ADC_ISR_EOS) != ADC_ISR_EOS);

	return ADC1->DR;
}


void ContinuousReadSingleChannel(ADCxPort *ADC_port) {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	RCC->AHBENR |= ADC_port->AHBENREnable;

	// set to synchronise the ADC with the clock
	ADC_port->ADCx_Common->CCR |= ADC_port->ClockMode;

	// ADEN must be = 0 for configuration (is the default)
	ADC_port->ADCx->CR &= ~ADC_port->VoltageRegulatorClear; // clear voltage regulator enable
	ADC_port->ADCx->CR |= ADC_port->VoltageRegulatorMode; // set ADVREGEN TO 01
	ADC_port->ADCx->CR &= ~ADC_port->DifferentialModeForCalibration; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC_port->ADCx->CR |= ADC_port->ADCCalibrate;
	while((ADC_port->ADCx->CR & ADC_port->ADCCalibrate) == ADC_port->ADCCalibrate); // Waiting for the calibration to finish

	// select the channel, only one conversion so it goes in
	//  the first conversion slot (SQ1)
	//  and the L value should be 0 (L=0000 does one conversion only)
	ADC_port->ADCx->SQR1 = 0; // clear any existing channel requests
	ADC_port->ADCx->SQR1 |= ADC_port->Channel1 << ADC_port->Channel1Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel2 << ADC_port->Channel2Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel3 << ADC_port->Channel3Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel4 << ADC_port->Channel4Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->NumChannels << ADC_port->NumChannelsPos; // set the number of channels to read

	// continuous mode
	ADC_port->ADCx->CFGR |= ADC_port->RunningMode;

	// Enable ADC
	ADC_port->ADCx->CR |= ADC_port->ADCEnable;

	while (ADC_port->ADCx->ISR == 0); //Wait the ADC to be ready.

	// request the process to start
	// only need once in continuous mode
	ADC_port->ADCx->CR |= ADC_port->ADCStart;

	ADC_port->value_1 = 0;
	ADC_port->scale_1 = 0;

    /* Loop forever */
	for(;;) {

		// Wait for the end of conversion
		while(!(ADC_port->ADCx->ISR &= ADC_ISR_EOC));

		ADC_port->value_1 = ADC_port->ADCx->DR;

		// read the value from ADC1->DR,
		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 8 even partitions (for 8 leds)
		ADC_port->scale_1 = ADC_port->value_1 / (0xfff / 8);

		if (ADC_port->scale_1 > 7) {
			ADC_port->scale_1 = 7;
		}

		*led_register = 1 << ADC_port->scale_1 ;
	}
}

void ContinuousReadSingleChannelADC2() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC2->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC2->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC2->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC2->CR |= ADC_CR_ADCAL;
	while((ADC2->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

	// select the channel, only one conversion so it goes in
	//  the first conversion slot (SQ1)
	//  and the L value should be 0 (L=0000 does one conversion only)
	ADC2->SQR1 = 0; // clear any existing channel requests
	ADC2->SQR1 |= 0x01 << ADC_SQR1_SQ1_Pos; // request channel 1
	ADC2->SQR1 |= 0x00 << ADC_SQR1_L_Pos; // set the number of channels to read

	// continuous mode
	ADC2->CFGR |= ADC_CFGR_CONT;

	// Enable ADC
	ADC2->CR |= ADC_CR_ADEN;

	while (ADC2->ISR == 0); //Wait the ADC to be ready.

	// request the process to start
	// only need once in continuous mode
	ADC2->CR |= ADC_CR_ADSTART;

	uint16_t value = 0;
	uint8_t scale = 0;

    /* Loop forever */
	for(;;) {

		// Wait for the end of conversion
		while(!(ADC2->ISR &= ADC_ISR_EOC));

		value = ADC2->DR;

		// read the value from ADC1->DR,
		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 8 even partitions (for 8 leds)
		scale = value / (0xfff / 2);

		if (scale > 2) {
			scale = 2;
		}

		*led_register = 1 << scale ;
	}
}

void ContinuousReadSingleChannelADC3() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// enable the clock for ADC1
	RCC->AHBENR |= RCC_AHBENR_ADC34EN;

	// set to synchronise the ADC with the clock
	ADC34_COMMON->CCR |= ADC34_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC3->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC3->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC3->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC3->CR |= ADC_CR_ADCAL;
	while((ADC3->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

	// select the channel, only one conversion so it goes in
	//  the first conversion slot (SQ1)
	//  and the L value should be 0 (L=0000 does one conversion only)
	ADC3->SQR1 = 0; // clear any existing channel requests
	ADC3->SQR1 |= 0x01 << ADC_SQR1_SQ1_Pos; // request channel 1
	ADC3->SQR1 |= 0x00 << ADC_SQR1_L_Pos; // set the number of channels to read

	// continuous mode
	ADC3->CFGR |= ADC_CFGR_CONT;

	// Enable ADC
	ADC3->CR |= ADC_CR_ADEN;

	while (ADC3->ISR == 0); //Wait the ADC to be ready.

	// request the process to start
	// only need once in continuous mode
	ADC3->CR |= ADC_CR_ADSTART;

	uint16_t value = 0;
	uint8_t scale = 0;

    /* Loop forever */
	for(;;) {

		// Wait for the end of conversion
		while(!(ADC3->ISR &= ADC_ISR_EOC));

		value = ADC3->DR;

		// read the value from ADC3->DR,
		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 8 even partitions (for 8 leds)
		scale = value / (0xfff / 8);

		if (scale > 7) {
			scale = 7;
		}

		*led_register = 1 << scale ;
	}
}

void SingleReadMultiChannelADC() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC1->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC1->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC1->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

/*
 	// can change the sampling time (longer is more accurate)
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP0_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP1_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP2_Pos;
*/

	// We want to read from two channels each sequence
	//  the first channel goes in SQ1
	//  the second channel goes in SQ2
	//  the number of channels to read = 2, so the L value is 1
	ADC1->SQR1 = 0;
	ADC1->SQR1 |= 0x02 << ADC_SQR1_SQ1_Pos; // set the request for channel 2
	ADC1->SQR1 |= 0x03 << ADC_SQR1_SQ2_Pos; // set the request for channel 3
	ADC1->SQR1 |= 0x01 << ADC_SQR1_L_Pos; // set the number of channels to read

	// single shot mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;

	// Enable the ADC
	ADC1->CR |= ADC_CR_ADEN;

	// Wait the ADC to be ready.
	while (ADC1->ISR == 0);

	uint16_t value_1 = 0;
	uint16_t value_2 = 0;

    /* Loop forever */
	for(;;) {

		// request the process to start
		ADC1->CR |= ADC_CR_ADSTART;

		// Wait for the end of the first conversion
		while(!(ADC1->ISR & ADC_ISR_EOC));

		// read the first value
		value_1 = ADC1->DR;

		while(!(ADC1->ISR & ADC_ISR_EOC));

		// read the second value
		value_2 = ADC1->DR;

		// reset the sequence flag
		ADC1->ISR |= ADC_ISR_EOS;


		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_1 = value_1 / (0xfff / 4);

		if (scale_1 > 3) {
			scale_1 = 3;
		}

		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_2 = value_2 / (0xfff / 4);

		if (scale_2 > 3) {
			scale_2 = 3;
		}

		// draw the state of channel 2 in the first 4 LEDs
		// and the state of channel 3 in the second set of 4 LEDs
		*led_register = 1 << scale_1 | 1 << (scale_2 + 4);
	}
}

void SingleReadMultiChannelADC1() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC1->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC1->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC1->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

/*
 	// can change the sampling time (longer is more accurate)
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP0_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP1_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP2_Pos;
*/

	// We want to read from two channels each sequence
	//  the first channel goes in SQ1
	//  the second channel goes in SQ2
	//  the number of channels to read = 2, so the L value is 1
	ADC1->SQR1 = 0;
	ADC1->SQR1 |= 0x02 << ADC_SQR1_SQ1_Pos; // set the request for channel 2
	ADC1->SQR1 |= 0x03 << ADC_SQR1_SQ2_Pos; // set the request for channel 3
	ADC1->SQR1 |= 0x04 << ADC_SQR1_SQ3_Pos; // set the request for channel 4
	ADC1->SQR1 |= 0x02 << ADC_SQR1_L_Pos; // set the number of channels to read

	// single shot mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;

	// Enable the ADC
	ADC1->CR |= ADC_CR_ADEN;

	// Wait the ADC to be ready.
	while (ADC1->ISR == 0);

	uint16_t value_1 = 0;
	uint16_t value_2 = 0;
	uint16_t value_3 = 0;

    /* Loop forever */
	for(;;) {

		// request the process to start
		ADC1->CR |= ADC_CR_ADSTART;

		// Wait for the end of the first conversion
		while(!(ADC1->ISR & ADC_ISR_EOC));

		// read the first value
		value_1 = ADC1->DR;

		while(!(ADC1->ISR & ADC_ISR_EOC));

		// read the second value
		value_2 = ADC1->DR;
/*
		while(!(ADC1->ISR & ADC_ISR_EOC));

		// read the second value
		value_3 = ADC1->DR;
*/
		// reset the sequence flag
		ADC1->ISR |= ADC_ISR_EOS;


		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_1 = value_1 / (0xfff / 2);

		if (scale_1 > 1) {
			scale_1 = 1;
		}

		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_2 = value_2 / (0xfff / 2);

		if (scale_2 > 1) {
			scale_2 = 1;
		}

		uint8_t scale_3 = value_3 / (0xfff / 2);

		if (scale_3 > 1) {
			scale_3 = 1;
		}

		// draw the state of channel 2 in the first 4 LEDs
		// and the state of channel 3 in the second set of 4 LEDs
		*led_register = 1 << scale_1 | 1 << (scale_2 + 2) | 1 << (scale_3 + 4);
	}
}

void SingleReadMultiChannelADC2() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// enable the clock for ADC1
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC2->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC2->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC2->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC2->CR |= ADC_CR_ADCAL;
	while((ADC2->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

/*
 	// can change the sampling time (longer is more accurate)
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP0_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP1_Pos;
	ADC1->SMPR1 |= 7 << ADC_SMPR1_SMP2_Pos;
*/

	// We want to read from two channels each sequence
	//  the first channel goes in SQ1
	//  the second channel goes in SQ2
	//  the number of channels to read = 2, so the L value is 1
	ADC2->SQR1 = 0;
	ADC2->SQR1 |= 0x01 << ADC_SQR1_SQ1_Pos; // set the request for channel 2
	ADC2->SQR1 |= 0x02 << ADC_SQR1_SQ2_Pos; // set the request for channel 3
	ADC2->SQR1 |= 0x03 << ADC_SQR1_SQ3_Pos; // set the request for channel 4
	ADC2->SQR1 |= 0x06 << ADC_SQR1_L_Pos; // set the number of channels to read //for three channels we need to read 7 channels for some dumb reason

	// single shot mode
	ADC2->CFGR &= ~ADC_CFGR_CONT;

	// Enable the ADC
	ADC2->CR |= ADC_CR_ADEN;

	// Wait the ADC to be ready.
	while (ADC2->ISR == 0);

	uint16_t value_1 = 0;
	uint16_t value_2 = 0;
	uint16_t value_3 = 0;
	//uint16_t value_4 = 0;



	for(;;) {

		// request the process to start
		ADC2->CR |= ADC_CR_ADSTART;

		// Wait for the end of the first conversion
		while(!(ADC2->ISR & ADC_ISR_EOC));

		// read the first value
		value_1 = ADC2->DR;

		while(!(ADC2->ISR & ADC_ISR_EOC));

		// read the second value
		value_2 = ADC2->DR;

		while(!(ADC2->ISR & ADC_ISR_EOC));

		// read the second value
		value_3 = ADC2->DR;

		//while(!(ADC2->ISR & ADC_ISR_EOC));

		// read the second value
		//value_4 = ADC2->DR;

		// reset the sequence flag
		ADC2->ISR |= ADC_ISR_EOS;


		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_1 = value_1 / (0xfff / 2);

		if (scale_1 > 1) {
			scale_1 = 1;
		}

		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 4 even partitions (for 4 leds)
		uint8_t scale_2 = value_2 / (0xfff / 2);

		if (scale_2 > 1) {
			scale_2 = 1;
		}

		uint8_t scale_3 = value_3 / (0xfff / 2);

		if (scale_3 > 1) {
			scale_3 = 1;
		}

		//uint8_t scale_4 = value_4 / (0xfff / 2);

		//if (scale_4 > 1) {
			//scale_4 = 1;
		//}

		// draw the state of channel 2 in the first 4 LEDs
		// and the state of channel 3 in the second set of 4 LEDs
		*led_register = 1 << scale_1 | 1 << (scale_2 + 2) | 1 << (scale_3 + 4); //| 1 << (scale_4 + 6);
	}
}

int conversion_counter = 0;



void enable_ADC12_interrupt() {
	__disable_irq();
	NVIC_SetPriority(ADC1_2_IRQn, 5);  // Set Priority
	NVIC_EnableIRQ(ADC1_2_IRQn);
	__enable_irq();
}

void enable_ADC3_interrupt() {
	__disable_irq();
	NVIC_SetPriority(ADC3_IRQn, 4);  // Set Priority
	NVIC_EnableIRQ(ADC3_IRQn);
	__enable_irq();
}

void toggle_EOC_interrupt(ADCxPort *ADC_port) {
	ADC_port->ADCx->IER ^= ADC_IER_EOCIE;
}

void toggle_ADRDY_interrupt(ADCxPort *ADC_port) {
	ADC_port->ADCx->IER ^= ADC_IER_ADRDYIE;
}

void toggle_EOS_interrupt(ADCxPort *ADC_port) {
	ADC_port->ADCx->IER ^= ADC_IER_EOSIE;
}


void ADCInitialise(ADCxPort *ADC_port) {
	RCC->AHBENR |= ADC_port->AHBENREnable;

	// set to synchronise the ADC with the clock
	ADC_port->ADCx_Common->CCR |= ADC_port->ClockMode;

	// ADEN must be = 0 for configuration (is the default)
	ADC_port->ADCx->CR &= ~ADC_port->VoltageRegulatorClear; // clear voltage regulator enable
	ADC_port->ADCx->CR |= ADC_port->VoltageRegulatorMode; // set ADVREGEN TO 01
	ADC_port->ADCx->CR &= ~ADC_port->DifferentialModeForCalibration; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC_port->ADCx->CR |= ADC_port->ADCCalibrate;
	while((ADC_port->ADCx->CR & ADC_port->ADCCalibrate) == ADC_port->ADCCalibrate); // Waiting for the calibration to finish

	// select the channel, only one conversion so it goes in
	//  the first conversion slot (SQ1)
	//  and the L value should be 0 (L=0000 does one conversion only)
	ADC_port->ADCx->SQR1 = 0; // clear any existing channel requests
	ADC_port->ADCx->SQR1 |= ADC_port->Channel1 << ADC_port->Channel1Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel2 << ADC_port->Channel2Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel3 << ADC_port->Channel3Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->Channel4 << ADC_port->Channel4Pos; // request channel 2
	ADC_port->ADCx->SQR1 |= ADC_port->NumChannels << ADC_port->NumChannelsPos; // set the number of channels to read

	// one shot mode
	ADC_port->ADCx->CFGR |= ADC_port->RunningMode;

	// Enable ADC
	ADC_port->ADCx->CR |= ADC_port->ADCEnable;
	//ADC1->CR |= ADC_CR_ADEN;

}

void SingleReadMultiChannelADCInterrupt(ADCxPort *ADC_port) {
	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
		// request the process to start
		ADC_port->ADCx->CR |= ADC_port->ADCStart;

		if (((ADC_port->ADCx->ISR & ADC_ISR_EOC) != 0) && (ADC_port->value_4 == 0)){
			if (ADC_port->conversion_counter == 0) {
					ADC_port->value_1 = ADC_port->ADCx->DR;
					ADC_port->conversion_counter++;
					ADC_port->value_2 = ADC_port->ADCx->DR;
					ADC_port->conversion_counter++;
					ADC_port->value_3 = ADC_port->ADCx->DR;
					ADC_port->conversion_counter++;
					ADC_port->value_4 = ADC_port->ADCx->DR;
					ADC_port->conversion_counter++;
			}
		}
		//Conversion Sequence has been finished
		if (((ADC_port->ADCx->ISR & ADC_ISR_EOS) != 0) && (ADC_port->conversion_counter > ADC_port->NumChannels)){
			//Reset sequence flag
			//ADC2->ISR &= ADC_ISR_EOS;

			//Get scaled values so we can put them into the display/LEDs
			ADC_port->scale_1 = ADC_port->value_1 / (0xfff / 2);

			if (ADC_port->scale_1 > 1) {
				ADC_port->scale_1 = 1;
			}

			// full range is 12 bits (0xFFF maximum)
			// divide the scale into 4 even partitions (for 4 leds)
			ADC_port->scale_2 = ADC_port->value_2 / (0xfff / 2);

			if (ADC_port->scale_2 > 1) {
				ADC_port->scale_2 = 1;
			}

			ADC_port->scale_3 = ADC_port->value_3 / (0xfff / 2);

			if (ADC_port->scale_3 > 1) {
				ADC_port->scale_3 = 1;
			}

			ADC_port->scale_4 = ADC_port->value_4 / (0xfff / 2);

			if (ADC_port->scale_4 > 1) {
				ADC_port->scale_4 = 1;
			}
			// draw the state of channel 2 in the first 4 LEDs
			// and the state of channel 3 in the second set of 4 LEDs
			*led_register = 1 << ADC_port->scale_1 | 1 << (ADC_port->scale_2 + 2) | 1 << (ADC_port->scale_3 + 4); //| 1 << (ADC_port->scale_4 + 6);
			ADC_port->value_1 = 0;
			ADC_port->value_2 = 0;
			ADC_port->value_3 = 0;
			ADC_port->value_4 = 0;
			ADC_port->conversion_counter = 0;
		}

}
