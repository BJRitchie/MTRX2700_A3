#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <stdint.h>
#include "stm32f303xc.h"

// Defining the hardware timer struct
struct _HardwareTimer;
typedef struct _HardwareTimer HardwareTimer;

// make instance for TIM2
extern HardwareTimer TIM2_init;

// instance for TIM4
extern HardwareTimer TIM4_init;

// initialise timer
// inputs:
// - hardware_timer: pointer to a struct containing
//	the initialisation variables for the desired timer
// - completion_function: a pointer to the function to be
// 	completed when an interrupt is encountered.
void TimerInitialise(HardwareTimer *hardware_timer, void (*completion_function)());

// enable the timer interrupts
void enable_timer_interrupt();

void set_continuous_mode(struct _HardwareTimer *hardware_timer, uint32_t period, void (*timer_callback)());

// get and set functions for timer

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - prescaler value of timer
uint16_t getPrescaleValue(struct _HardwareTimer *hardware_timer);

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired prescaler to be set
void setPrescaleValue(struct _HardwareTimer *hardware_timer, uint16_t value);

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - ARRValue
uint32_t getARRValue(struct _HardwareTimer *hardware_timer);

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired ARRValue to be set
void setARRValue(struct _HardwareTimer *hardware_timer, uint32_t value);

// Set a period between clock overflows and therefore interrupts .
// Will also reset clock.
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - period: the delay period in microseconds
void setPeriod(struct _HardwareTimer *hardware_timer, uint32_t period);

// set the completion function for the hardware timer in question
// - hardware_timer: pointer to the hardware timer where the period is being set
// - completion_function: a pointer to the function to be run when interrupt is reached
void setCompletionFunc(struct _HardwareTimer *hardware_timer, void (*completion_function)());

// function to set hardware_timer to one-shot-mode
// - hardware_timer: pointer to the hardware timer in question
// - delay: the delay before the timer is triggered in microseconds
// note: TimerInitialise should be used first.
void set_one_shot_mode(struct _HardwareTimer *hardware_timer, uint32_t delay, void (*completion_function)());

#endif
