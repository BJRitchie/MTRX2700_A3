#include <string.h>
#include "timer.h"
#include "stm32f303xc.h"
#include "adc.h"

#define TRUE 1
#define FALSE 0
#define CLOCK_FREQUENCY 8000000

struct _HardwareTimer {
	TIM_TypeDef *TIMx;
	volatile uint16_t PrescaleValue;	// prescaler value
	volatile uint32_t ARRValue; 		// prescaler value
	volatile uint8_t oneShotMode; 		// TRUE or FALSE
	volatile uint32_t MaskAPB2ENR;		// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;		// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;		// mask to enable RCC AHB bus registers
	volatile uint32_t MaskCR1; 			// mask to apply to CR1 (control reg 1)
	volatile uint32_t MaskDIER; 		// mask to apply to DIER (DMA/interrupt enable register)
	void (*timer_completion_function)(); 	// the the code block to be run when the interrupt is flagged
};

HardwareTimer TIM2_init = {
		TIM2,
		7999, 				// default prescaler value
		1000,				// overflow value
		FALSE, 				// not in one-shot mode
		0x00, 				// bit to enable APB2 bus
		RCC_APB1ENR_TIM2EN, // bit to enable APB1 bus
		0x00, 				// bit to enable for AHB bus
		TIM_CR1_URS, 		// bit for CR1 reg; interrupt only flagged at over/underflow
		TIM_DIER_UIE, 		// enable update interrupt
		0x00,
};

HardwareTimer TIM4_init = {
		TIM4,
		7999, 				// default prescaler value
		1000, 				// default overflow value
		FALSE, 				// not in one-shot mode
		0x00, 				// bit to enable APB2 bus
		RCC_APB1ENR_TIM4EN, // bit to enable APB1 bus
		0x00, 				// bit to enable for AHB bus
		TIM_CR1_URS, 		// bit for CR1 reg; interrupt only flagged at over/underflow
		TIM_DIER_UIE, 		// enable update interrupt
		0x00,
};

// initialise a hardware timer
void TimerInitialise(HardwareTimer *hardware_timer, uint32_t delay, void (*completion_function)()) {

    // Enable clock
    RCC->APB1ENR |= hardware_timer->MaskAPB1ENR;

    // set prescaler
    hardware_timer->PrescaleValue = 7999; // bring clock speed down to 1kHz
    hardware_timer->TIMx->PSC = hardware_timer->PrescaleValue;

    // set auto reload register to delay time
    hardware_timer->ARRValue = delay;
    hardware_timer->TIMx->ARR = hardware_timer->ARRValue;

    // set CR1 mask
    hardware_timer->TIMx->CR1 |= hardware_timer->MaskCR1;

    // set DIER mask
    hardware_timer->TIMx->DIER |= hardware_timer->MaskDIER;

    // set the timer completion function
    hardware_timer->timer_completion_function = completion_function;

    // start TIMx
    hardware_timer->TIMx->CR1 |= TIM_CR1_CEN;
}


// tim2 overflow handler
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF; // Clear the interrupt flag

        if (TIM2_init.oneShotMode) {
            TIM2->CR1 &= ~TIM_CR1_CEN; // Stop the timer if in one-shot mode

            TIM2_init.oneShotMode = 0; // turn off the oneshot mode
        }

        if (TIM2_init.timer_completion_function != NULL) {
            TIM2_init.timer_completion_function();
        }
    }
}

// tim4 overflow handler
void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF; // Clear the interrupt flag

        if (TIM4_init.oneShotMode) {

        	TIM4->CR1 &= ~TIM_CR1_CEN; // Stop the timer if in one-shot mode

            TIM4_init.oneShotMode = 0; // turn off the oneshot mode
        }

        if (TIM4_init.timer_completion_function != NULL) {
            TIM4_init.timer_completion_function();
        }
    }
}


// enable the timer overflow interrupt
void enable_timer_interrupt() {
    // disable interrupts
	__disable_irq();

	NVIC_SetPriority(TIM2_IRQn, 3); // Set priority
    NVIC_EnableIRQ(TIM2_IRQn); 		// Enable TIM2 IRQ

	NVIC_SetPriority(TIM4_IRQn, 2); // Set priority
    NVIC_EnableIRQ(TIM4_IRQn); 		// Enable TIM4 IRQ

    // re-enable interrupts
    __enable_irq();
}


// getter and setter functions for timer

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - prescaler value of timer
uint16_t getPrescaleValue(struct _HardwareTimer *hardware_timer){
	return hardware_timer->PrescaleValue;
}

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired prescaler to be set
void setPrescaleValue(struct _HardwareTimer *hardware_timer, uint16_t value) {
	// set value
	hardware_timer->PrescaleValue = value;
    hardware_timer->TIMx->PSC = value;

	// reset timer to engage clock
	hardware_timer->TIMx->EGR |= 0x01;
}

// gets the ARRValue
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// ouput:
// - ARRValue
uint32_t getARRValue(struct _HardwareTimer *hardware_timer){
	return hardware_timer->ARRValue;
}

// set the auto-reload register value
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - value: the desired ARRValue to be set
void setARRValue(struct _HardwareTimer *hardware_timer, uint32_t value) {
	// set value
	hardware_timer->ARRValue = value;
    hardware_timer->TIMx->ARR = value;
}

// Set a period between clock overflows and therefore interrupts .
// Will also reset clock.
// input:
// - hardware_timer: pointer to the hardware timer where the period is being set
// - period: the delay period in milliseconds
void setPeriod(struct _HardwareTimer *hardware_timer, uint32_t period) {
	setPrescaleValue(hardware_timer, 8000-1); 	// make clock freq 1kHz
	setARRValue(hardware_timer, period);  	// set overflow to period value
}

// gets the current period
uint32_t getPeriod(struct _HardwareTimer *hardware_timer) {
	uint32_t period = 0;
	// get the reload register value
	uint32_t arr_val = getARRValue(hardware_timer);

	// get the prescaler value
	uint16_t psc_val = getPrescaleValue(hardware_timer);

	if (psc_val == 7999) {
		period = arr_val; // in millisecs
	}

	return period;
}


void setCompletionFunc(struct _HardwareTimer *hardware_timer, void (*completion_function)()){
	hardware_timer->timer_completion_function = completion_function;
}


void setOneShotMode(struct _HardwareTimer *hardware_timer, uint32_t t_delay, void (*completion_function)()) {
	// set the one shot mode to 1
	hardware_timer->oneShotMode = 1;

	// initialise the timer
	TimerInitialise(hardware_timer, t_delay, completion_function);

	// set delay
	setPeriod(hardware_timer, t_delay);
}

void setContinuousMode(struct _HardwareTimer *hardware_timer, uint32_t period, void (*timer_callback)()) {

    // Ensure one-shot mode is disabled
    hardware_timer->oneShotMode = 0;

	// initialise the timer
	TimerInitialise(hardware_timer, period, timer_callback);

	// set period
	setPeriod(hardware_timer, period);
}
