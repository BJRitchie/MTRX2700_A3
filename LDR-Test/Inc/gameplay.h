#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stdint.h>
#include <adc.h>
#include <stm32f303xc.h>

struct _GameData {
	volatile uint8_t StartValue; //How many seconds we need to hold before game starts
	volatile uint8_t StartCounter; //The timer x ADC3 start counter
	volatile uint8_t GamePhaseValue; //Tells what phase of the game we are in, pre 1, playing 2, post 3
	volatile uint8_t Phase2Timer;
	volatile uint8_t Phase2TimeLimit;
};

typedef struct _GameData GameData;

extern GameData game_data1;

//Spins leds and counts them up to start the game, or not
void spin_and_count_leds(ADCxPort *ADC, GameData *game_data);

//Countdown timer for gameplay
void GameTimer(ADCxPort *ADC, GameData *game_data);

#endif
