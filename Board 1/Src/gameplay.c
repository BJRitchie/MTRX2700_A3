#include <gameplay.h>
#include <stdint.h>
#include <stm32f303xc.h>
#include <adc.h>
#include <timer.h>
#include <serial.h>
#include <stdio.h>



GameData game_data1 = {
	1, //Start Time Limit
	0, //Start Counter
	1, //Game Phase 1, 2, or 3
	1, //Timer mode 1, 2, or 3
	0, //Timer
	100000, //Time limit
	0, //Three Checkpoints start off
	0,
	0,
	0,//Default is user losing
};

void (*ADC12_interrupt)(ADCxPort *ADC_port) = 0x00;
void (*ADC3_interrupt)(ADCxPort *ADC_port) = 0x00;
void (*TIM2_interrupt)(GameData *game_data1) = 0x00;
void (*on_timer2_complete)(ADCxPort *ADC_port, GameData *game_data1) = 0x00;
void (*on_button_press)() = 0x00;

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;

	// enable the clock for ADC1
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;
	RCC->AHBENR |= RCC_AHBENR_ADC34EN;
}

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {

	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

void ADC1_2_IRQHandler() {
	if (ADC12_interrupt != 0x00) {
		ADC12_interrupt(&ADC2_Port);
	}
}

void ADC3_IRQHandler() {

	if (ADC3_interrupt != 0x00) {
		ADC3_interrupt(&ADC3_Port);
	}
}

//Initialises Game

void EXTI0_IRQHandler(void) {
	// run the button press handler (make sure it is not null first !)
	if (on_button_press != 0x00) {
		on_button_press(&ADC2_Port, &game_data1);
	}

	if (game_data1.GamePhaseValue == 1){
		Phase1();
	}


	// reset the interrupt (so it doesn't keep firing until the next trigger)
	EXTI->PR |= EXTI_PR_PR0;
}

void EnableEXTIInterrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// Enable the system configuration controller (SYSCFG in RCC)
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// External Interrupts details on large manual page 294)
	// PA0 is on interrupt EXTI0 large manual - page 250
	// EXTI0 in  SYSCFG_EXTICR1 needs to be 0x00 (SYSCFG_EXTICR1_EXTI0_PA)
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

	//  Select EXTI0 interrupt on rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0; // rising edge of EXTI line 0 (includes PA0)

	// set the interrupt from EXTI line 0 as 'not masked' - as in, enable it.
	EXTI->IMR |= EXTI_IMR_MR0;

	// Tell the NVIC module that EXTI0 interrupts should be handled
	NVIC_SetPriority(EXTI0_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ(EXTI0_IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void Timer2CompletionFunction() {
	if (game_data1.GamePhaseValue == 1) {
		on_timer2_complete = &spin_and_count_leds;
		if (on_timer2_complete != 0x00) {
			on_timer2_complete(&ADC3_Port, &game_data1);
		}
	}
	else if (game_data1.GamePhaseValue == 2) {
		on_timer2_complete = &GameTimer;
		if (on_timer2_complete != 0x00) {
			on_timer2_complete(&ADC2_Port, &game_data1);
		}
	}
	else if (game_data1.GamePhaseValue == 3) {
		on_timer2_complete = &FlashEndLights;
		if (on_timer2_complete != 0x00) {
			on_timer2_complete(&ADC2_Port, &game_data1);
		}
	}

}

void Timer4CompletionFunction() {
	uint8_t string_to_send[64] = "This is a string !\r\n";
	sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd\r\n", game_data1.GamePhaseValue , game_data1.Checkpoint1, game_data1.Checkpoint2, game_data1.Checkpoint3, game_data1.WonOrLost);
	SerialOutputString(string_to_send, &USART1_PORT);
	if (game_data1.TimerMode == 1) {
		if (ADC2_Port.scale_1 == 1){
			game_data1.Checkpoint1 = 1;
		}
		if (ADC2_Port.scale_2 == 1){
			game_data1.Checkpoint2 = 1;
		}
		if (ADC2_Port.scale_3 == 1){
			game_data1.Checkpoint3 = 1;
		}
		//Check if checkpoints have been passed
		if ((game_data1.Checkpoint1 && game_data1.Checkpoint2 && game_data1.Checkpoint3) == 1){
			game_data1.GamePhaseValue = 3;
			game_data1.WonOrLost = 1;
			FlashEndLights(&ADC2_Port, &game_data1);
		}
	}
	else if (game_data1.TimerMode == 2) {
		if (ADC2_Port.scale_1 == 1){
			game_data1.Checkpoint1 = 1;
		}
		if (ADC2_Port.scale_2 == 1){
			game_data1.Checkpoint2 = 1;
		}
		if (ADC2_Port.scale_3 == 1){
			game_data1.Checkpoint3 = 1;
		}
		//Check if checkpoints have been passed
		if ((game_data1.Checkpoint1 && game_data1.Checkpoint2 && game_data1.Checkpoint3) == 1){
			game_data1.GamePhaseValue = 3;
			game_data1.WonOrLost = 1;
			FlashEndLights(&ADC2_Port, &game_data1);
		}

	}

}

void Phase1(){


	enable_clocks();
	initialise_board();

	SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);
	uint8_t string_to_send[64] = "This is a string !\r\n";
	sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd\r\n", game_data1.GamePhaseValue , game_data1.Checkpoint1, game_data1.Checkpoint2, game_data1.Checkpoint3, game_data1.WonOrLost);
	SerialOutputString(string_to_send, &USART1_PORT);

	on_button_press = &RestartGame;
	EnableEXTIInterrupt();
	enable_timer_interrupt();

	ADC3_interrupt = &SingleReadMultiChannelADCInterrupt;
	enable_ADC3_interrupt();
	toggle_EOC_interrupt(&ADC3_Port);
	toggle_ADRDY_interrupt(&ADC3_Port);
	toggle_EOS_interrupt(&ADC3_Port);

	on_timer2_complete = &spin_and_count_leds;
	uint32_t period = 1000; // ms
	TimerInitialise(&TIM2_init, period, &Timer2CompletionFunction);
	ADCInitialise(&ADC3_Port);
}

void Phase2(){
	toggle_EOC_interrupt(&ADC3_Port);
	toggle_ADRDY_interrupt(&ADC3_Port);
	toggle_EOS_interrupt(&ADC3_Port);

	uint8_t string_to_send[64] = "This is a string !\r\n";
	sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd\r\n", game_data1.GamePhaseValue , game_data1.Checkpoint1, game_data1.Checkpoint2, game_data1.Checkpoint3, game_data1.WonOrLost);
	SerialOutputString(string_to_send, &USART1_PORT);

	ADC12_interrupt = &SingleReadMultiChannelADCInterrupt;
	enable_ADC12_interrupt();
	toggle_EOC_interrupt(&ADC2_Port);
	toggle_ADRDY_interrupt(&ADC2_Port);
	toggle_EOS_interrupt(&ADC2_Port);

	on_timer2_complete = &spin_and_count_leds;
	uint32_t period = 10; // ms
	TimerInitialise(&TIM4_init, period, &Timer4CompletionFunction);
	ADCInitialise(&ADC2_Port);


}


void spin_and_count_leds(ADCxPort *ADC, GameData *game_data){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	int position = 0;


	if (ADC->scale_1 == 1){
		game_data->StartCounter++;
	}
	else {
		game_data->StartCounter = 0;
	}

	if (game_data->StartCounter >= game_data->StartValue){
		for (int i = 0; i <= game_data->StartCounter; i++) {
			*led_register |= 1 << (8 - i);
			game_data->GamePhaseValue = 2;
		}
		//Flash leds to Start
		for (uint32_t i = 0; i < 0xAFFF; i++);
	}
}

void GameTimer(ADCxPort *ADC, GameData *game_data){
	//Changing to Game Phase 2 settings
	if ((ADC->ADCx->CR & ADC->ADCEnable) == 0) {
		Phase2();
	}
	if (game_data->Phase2Timer == game_data->Phase2TimeLimit){
		game_data->GamePhaseValue = 3;
		game_data->WonOrLost = 0;
	}
	else {
		game_data->Phase2Timer++;
	}
}

void FlashEndLights(ADCxPort *ADC, GameData *game_data){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	uint8_t led_state = 0x00;

	uint8_t string_to_send[64] = "This is a string !\r\n";
	sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd\r\n", game_data1.GamePhaseValue , game_data1.Checkpoint1, game_data1.Checkpoint2, game_data1.Checkpoint3, game_data1.WonOrLost);
	SerialOutputString(string_to_send, &USART1_PORT);

	while (game_data->GamePhaseValue == 3){
		//Spin Leds and stack them
		for (int j = 0; j <= 7; j ++) {
			for (int i = 0; i <= 7; i++){
				*led_register = led_state;
				*led_register |= 1 << i;
				if (i == 7 - j){
					*led_register |= 1 << (7 - j);
					led_state |= 1 << (7 - j);
				}
				for (uint32_t wait = 0; wait < (0xAFFF/(8-j)); wait++);
			}
		}
		//Spins leds and unstacks them
		for (uint32_t wait = 0; wait < (0xFFFF/(16)); wait++);
		int backwards_count = 7;
		for (int j = 0; j <= 7; j++) {
			int led_removal_number = 0;
			for (int i = 7; i >= 0; i--) {
				*led_register = led_state;
				*led_register ^= 1 << (7 - backwards_count - led_removal_number);
				if (i == 7 - j){
					led_state ^= 1 << (7 - i);
				}
				led_removal_number++;
				for (uint32_t wait = 0; wait < (0xAFFF/(8-backwards_count)); wait++);
			}
			backwards_count--;
		}

		//Spins leds
		for (int j = 0; j <= 7; j++) {
			for (int i = 0; i <= 7; i++) {
				*led_register = led_state;
				*led_register ^= 1 << i;
				if (i == 1 + j){
					*led_register ^= 0 << (1 + j);
					led_state ^= 0 << (1 + j);
				}
				for (uint32_t wait = 0; wait < (0xAFFF/(8-j)); wait++);
			}
		}

		led_state = 0x00;
		//This makes the leds jump one by one slow to fast
		*led_register = 0x00;
		for (uint32_t wait = 0; wait < (0xAFFF/(1)); wait++);
		backwards_count = 8;
		for (int j = 0; j <= 8; j++) {
			for (int i = 0; i <= 8; i++) {
				*led_register = led_state;
				//*led_register &= 0 << i;
				if (i == j){
					if (j == 0) {
						*led_register |= 1 << (7);
						led_state &= 1 << (7);
					}
					else {
						*led_register |= 1 << (j-1);
						led_state &= 1 << (j-1);
					}
				}
				for (uint32_t wait = 0; wait < (0xAFFF/(9-backwards_count)); wait++);
			}
			backwards_count--;
		}

		//Reset the leds
		led_state = 0x00;
	}
}

void RestartGame(ADCxPort *ADC, GameData *game_data){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	//Flash Leds
	*led_register = 0x00;
	for (uint32_t i = 0; i < 0x1FFFF; i++);
	*led_register = 0xFF;
	for (uint32_t i = 0; i < 0x1FFFF; i++);
	*led_register = 0x00;
	for (uint32_t i = 0; i < 0x1FFFF; i++);
	*led_register = 0xFF;
	for (uint32_t i = 0; i < 0x1FFFF; i++);
	*led_register = 0x00;
	for (uint32_t i = 0; i < 0x1FFFF; i++);
	game_data->Checkpoint1 = 0;
	game_data->Checkpoint2 = 0;
	game_data->Checkpoint3 = 0;
	game_data->Phase2Timer = 0;
	game_data->StartCounter = 0;
	game_data->GamePhaseValue = 1;

	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_ClearPendingIRQ(TIM4_IRQn);
	NVIC_ClearPendingIRQ(ADC1_2_IRQn);
	NVIC_ClearPendingIRQ(ADC3_IRQn);
	NVIC_DisableIRQ(TIM2_IRQn);
	NVIC_DisableIRQ(TIM4_IRQn);
	NVIC_DisableIRQ(ADC1_2_IRQn);
	NVIC_DisableIRQ(ADC3_IRQn);
  	NVIC_SystemReset();
}


