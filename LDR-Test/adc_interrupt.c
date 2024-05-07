#include <adc_interrupt.h>
#include <stm32f303xc.h>

int conversion_counter = 0;



void enable_ADC12_interrupt() {
	__disable_irq();
	NVIC_SetPriority(ADC1_2_IRQn, 3);  // Set Priority
	NVIC_EnableIRQ(ADC1_2_IRQn);
	__enable_irq();
}

void enable_ADC3_interrupt() {
	__disable_irq();
	NVIC_SetPriority(ADC3_IRQn, 2);  // Set Priority
	NVIC_EnableIRQ(ADC3_IRQn);
	__enable_irq();
}

void toggle_EOC_interruptADC12() {
	ADC1->IER ^= ADC_IER_EOCIE;
	ADC2->IER ^= ADC_IER_EOCIE;
}

void toggle_EOC_interruptADC3() {
	ADC3->IER ^= ADC_IER_EOCIE;
}

void toggle_ADRDY_interruptADC12() {
	ADC1->IER ^= ADC_IER_ADRDYIE;
	ADC2->IER ^= ADC_IER_ADRDYIE;
}

void toggle_ADRDY_interruptADC3() {
	ADC3->IER ^= ADC_IER_ADRDYIE;
}

void toggle_EOS_interruptADC12() {
	ADC1->IER ^= ADC_IER_EOSIE;
	ADC2->IER ^= ADC_IER_EOSIE;
}

void toggle_EOS_interruptADC3() {
	ADC3->IER ^= ADC_IER_EOSIE;
}

void ADC2Initialise() {
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
	ADC2->SMPR1 |= 7 << ADC_SMPR1_SMP0_Pos;
	ADC2->SMPR1 |= 7 << ADC_SMPR1_SMP1_Pos;
	ADC2->SMPR1 |= 7 << ADC_SMPR1_SMP2_Pos;
*/

	// We want to read from two channels each sequence
	//  the first channel goes in SQ1
	//  the second channel goes in SQ2
	//  the number of channels to read = 2, so the L value is 1
	ADC2->SQR1 = 0;
	ADC2->SQR1 |= 0x01 << ADC_SQR1_SQ1_Pos; // set the request for channel 2
	ADC2->SQR1 |= 0x02 << ADC_SQR1_SQ2_Pos; // set the request for channel 3
	ADC2->SQR1 |= 0x03 << ADC_SQR1_SQ2_Pos; // set the request for channel 3
	ADC2->SQR1 |= 0x06 << ADC_SQR1_L_Pos; // set the number of channels to read

	// single shot mode
	ADC2->CFGR &= ~ADC_CFGR_CONT;

	// Enable the ADC
	ADC2->CR |= ADC_CR_ADEN;

}

void SingleReadMultiChannelADCInterrupt() {
	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	//If we haven't yet started the ADC, we need to start it
	//if ((ADC2->ISR & ADC_ISR_OVR) != 0) {
		//ADC2->ISR ^= 1 << ADC_ISR_OVR_Pos;
	//}
	//if ((ADC2->ISR & ADC_ISR_ADRDY) == 0){
	//}

	//If the ADC has been started and we can now take values from the DR
	//if ((ADC2->ISR & ADC_ISR_ADRDY) != 0){

		// request the process to start
		ADC2->CR |= ADC_CR_ADSTART;

		//set places for the ADC data to go
		uint16_t value_1 = 0;
		uint16_t value_2 = 0;
		uint16_t value_3 = 0;

		if (value_3 == 0) {
			//toggle
		}


		if ((ADC2->ISR & ADC_ISR_EOC) != 0){
			if (conversion_counter == 0){
				value_1 = ADC2->DR;

				value_2 = ADC2->DR;

				value_3 = ADC2->DR;
			}
		}
		//Conversion Sequence has been finished
		if ((ADC2->ISR & ADC_ISR_EOS) != 0){
			//Reset sequence flag
			//ADC2->ISR &= ADC_ISR_EOS;

			//Get scaled values so we can put them into the display/LEDs
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

	//}

}
