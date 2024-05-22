#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stdint.h>
#include <adc.h>
#include <stm32f303xc.h>

void run_game();

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks();

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board();

//ADC 1 and 2 interrupt handler
void ADC1_2_IRQHandler();

//ADC 3 interrupt handler
void ADC3_IRQHandler();

//EXTI0 interrupt handler
void EXTI0_IRQHandler(void);

//Enables the EXTI0 interrupt
void EnableEXTIInterrupt();

//
void Timer2CompletionFunction();

void Phase1();

void Phase2();

struct _GameData {
	volatile uint8_t StartValue; //How many seconds we need to hold before game starts
	volatile uint8_t StartCounter; //The timer x ADC3 start counter
	volatile uint8_t GamePhaseValue; //Tells what phase of the game we are in, pre 1, playing 2, post 3
	volatile uint8_t TimerMode; //Choose to be in countdown mode or speed run mode, or time trial mode
	volatile uint8_t Phase2Timer;
	volatile uint8_t Phase2TimeLimit; //Game Time limit
	volatile uint8_t Checkpoint1;
	volatile uint8_t Checkpoint2;
	volatile uint8_t Checkpoint3;
	volatile uint8_t WonOrLost; //Value if user wins or loses
};

typedef struct _GameData GameData;

extern GameData game_data1;

//Spins leds and counts them up to start the game, or not
void spin_and_count_leds(ADCxPort *ADC, GameData *game_data);

//Countdown timer for gameplay
void GameTimer(ADCxPort *ADC, GameData *game_data);

//Flashes lights in a cool pattern when the game is done
void FlashEndLights(ADCxPort *ADC, GameData *game_data);

//Restarts game to phase 1, resets game data used and flashes some lights
void RestartGame(ADCxPort *ADC, GameData *game_data);

#endif
