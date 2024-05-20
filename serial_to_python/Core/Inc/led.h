#ifndef LED_HEADER
#define LED_HEADER

#include <stdint.h>

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_leds();

// code from lecture example W05-C-interrupt by Dr Stewart Worrall
// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks();

// chase the led around a circle
void chase_led();

// turn on all of the leds
void led_on();

// turn off all of the leds
void led_off();

// chases an led in a circle with increasing speed
void chase_led_changing_period();

//spins leds around in a circle once
void spin_leds();

#endif
