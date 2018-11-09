/*
 * CFile1.c
 *
 * Created: 02-11-2018 15:15:28
 *  Author: lars-p
 */ 

#include "ADCTimer_drv.h"

void ADCtimerSetup() //Find new name !!
{
	ADC_setup();
	ADC_pin1();
	ADC_pin2();
	ADC_pin3();
	
	Timer0_CTC1();
	Timer0_CTC2();
	Timer0_interrupt();
	Timer0_1ms();
}

