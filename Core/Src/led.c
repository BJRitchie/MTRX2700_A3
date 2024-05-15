#include "led.h"
#include "timer.h"
#include "stm32f303xc.h"

// chase the led around the circle (also by Dr Stewart Worrall)
void chase_led(){
	// get the period
	uint32_t period = getARRValue(&TIM2_init);
	period -= 1000;
	if (period < 100) {
		period = 100000; // reset
	}
	setPeriod(&TIM2_init, period);

	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}

void led_on(void) {
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0x00;
	*led_register = 0xFF;
	for (uint32_t i = 0; i < 0xAFFFF; i++);
}

void led_off(void) {
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0x00;
}

// chase the led around the circle while decreasing the period
void chase_led_changing_period(){
	// get the period
	uint32_t period = getPeriod(&TIM2_init);

	period -= 25;
	if (period <= 25) {
		period = 1000; // reset
	}
	setPeriod(&TIM2_init, period );

	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1;
	if (*led_register == 0) {
		*led_register = 1;
	}
}

void spin_leds(){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	int position = 0;
	while (position <= 7) {
		*led_register = 0x00;
		*led_register |= 1 << position;
		position++;
		for (uint32_t i = 0; i < 0xAFFF; i++);
	}
}



