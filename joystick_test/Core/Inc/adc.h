#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stm32f303xc.h>

struct _ADCxPort {
	ADC_TypeDef *ADCx;
	ADC_Common_TypeDef *ADCx_Common;
	volatile uint32_t AHBENREnable;
	volatile uint32_t ClockMode;
	volatile uint32_t VoltageRegulatorClear;
	volatile uint32_t VoltageRegulatorMode;
	volatile uint32_t DifferentialModeForCalibration;
	volatile uint32_t ADCCalibrate;
	volatile uint32_t NumChannels;
	volatile uint32_t NumChannelsPos;
	volatile uint32_t Channel1;
	volatile uint32_t Channel2;
	volatile uint32_t Channel3;
	volatile uint32_t Channel4;
	volatile uint32_t Channel1Pos;
	volatile uint32_t Channel2Pos;
	volatile uint32_t Channel3Pos;
	volatile uint32_t Channel4Pos;
	volatile uint32_t RunningMode;
	volatile uint32_t ADCEnable;
	volatile uint32_t ADCStart;
	volatile uint16_t value_1;
	volatile uint16_t value_2;
	volatile uint16_t value_3;
	volatile uint16_t value_4;
	volatile uint16_t conversion_counter;
	volatile uint16_t scale_1;
	volatile uint16_t scale_2;
	volatile uint16_t scale_3;
	volatile uint16_t scale_4;
};

typedef struct _ADCxPort ADCxPort;

extern ADCxPort ADC1_Port;
extern ADCxPort ADC2_Port;
extern ADCxPort ADC3_Port;

void sendToServos();

void run();

uint16_t ReadADC(uint32_t channel);

void ContinuousReadSingleChannel(ADCxPort *ADC_port);

void ContinuousReadSingleChannelADC2();

void ContinuousReadSingleChannelADC3();

void getJoystickPosition();

void SingleReadMultiChannelADC();

void SingleReadMultiChannelADC1();

void SingleReadMultiChannelADC2();

void ADC1_2_IRQHandler();

void ADC3_IRQHandler();

void enable_ADC12_interrupt();

void enable_ADC3_interrupt();

void toggle_EOC_interrupt(ADCxPort *ADC_port);

void toggle_ADRDY_interrupt(ADCxPort *ADC_port);

void toggle_EOS_interrupt(ADCxPort *ADC_port);

void ADCInitialise(ADCxPort *ADC_port);

void SingleReadMultiChannelADCInterrupt(ADCxPort *ADC_port);

#endif

