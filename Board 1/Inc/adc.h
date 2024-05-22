#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stm32f303xc.h>

// ADC port structure containing LDR read values and initialise values
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

//ADC ports 
extern ADCxPort ADC1_Port;
extern ADCxPort ADC2_Port;
extern ADCxPort ADC3_Port;

//Reads an ADC to a given channel
uint16_t ReadADC(uint32_t channel);

// Reads the ADC on continuous mode, pass in an ADC port struct to use a specific ADC port
void ContinuousReadSingleChannel(ADCxPort *ADC_port);

// Reads the ADC2 on continuous mode
void ContinuousReadSingleChannelADC2();

// Reads the ADC3 on continuous mode
void ContinuousReadSingleChannelADC3();

//One shot ADC1 read with multiple channels
void SingleReadMultiChannelADC1();

//One shot ADC2 read with multiple channels
void SingleReadMultiChannelADC2();

//ADC 1 and 2 interrupt handler
void ADC1_2_IRQHandler();

//ADC 3 interrupt handler
void ADC3_IRQHandler();

//Enables ADC 1 and 2 global interrupt
void enable_ADC12_interrupt();

//Enables ADC 3 global interrupt
void enable_ADC3_interrupt();

//toggles the end of conversion interrupt for a given ADC port
void toggle_EOC_interrupt(ADCxPort *ADC_port);

//toggles the ADC ready interrupt for a given ADC port
void toggle_ADRDY_interrupt(ADCxPort *ADC_port);

//toggles the end of sequence interrupt for a given ADC port
void toggle_EOS_interrupt(ADCxPort *ADC_port);

//Initiliase a given ADC
void ADCInitialise(ADCxPort *ADC_port);

//One shot ADC read for a given ADC port using interrupts and multiple channels
void SingleReadMultiChannelADCInterrupt(ADCxPort *ADC_port);

#endif

