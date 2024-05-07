#ifndef ADC_INTERRUPT_H
#define ADC_INTERRUPT_H

#include <stdint.h>


void ADC1_2_IRQHandler();

void ADC3_IRQHandler();

void enable_ADC12_interrupt();

void enable_ADC3_interrupt();

void enable_EOC_interruptADC12();

void enable_EOC_interruptADC3();

void enable_ADRDY_interruptADC12();

void enable_ADRDY_interruptADC3();

void enable_EOS_interruptADC12();

void enable_EOS_interruptADC3();

void ADC1Initialise();

void SingleReadMultiChannelADCInterrupt();



#endif
