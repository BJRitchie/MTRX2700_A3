#include "serial.h"
#include "adc.h"
#include "gameplay.h"
#include "timer.h"
#include "led.h"
#include "stdint.h"
#include "stm32f303xc.h"
#include "math.h"

int main(void)
{
	Phase1();
	for(;;);
}

