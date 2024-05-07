#include <gameplay.h>
#include <stdint.h>
#include <stm32f303xc.h>
#include <adc.h>

GameData game_data1 = {
	3,
	0,
	1,
	0,
	3,
};

void spin_and_count_leds(ADCxPort *ADC, GameData *game_data){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	int position = 0;


	if (ADC->scale_1 == 1){
		game_data->StartCounter++;
	}
	else {
		game_data->StartCounter = 0;
	}

	/*
	while (position <= 7) {
		*led_register = 0x00;
		*led_register |= 1 << position;
		position++;
		for (int i = 0; i <= game_data1.StartCounter; i++) {
			*led_register |= 1 << (8 - i);
		}

		for (uint32_t i = 0; i < 0x4FFF; i++);

	}
	*/
	if (game_data->StartCounter >= game_data->StartValue){
		for (int i = 0; i <= game_data->StartCounter; i++) {
			*led_register |= 1 << (8 - i);
			game_data->GamePhaseValue = 2;
		}
		//Flash leds to Start
		for (uint32_t i = 0; i < 0xAFFF; i++);
		game_data->StartCounter = 0;
	}
}

void GameTimer(ADCxPort *ADC, GameData *game_data){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	if (game_data->Phase2Timer == game_data->Phase2TimeLimit){
		*led_register = 0xFF;
		for(;;);
	}
	else {
		game_data->Phase2Timer++;
	}

}
