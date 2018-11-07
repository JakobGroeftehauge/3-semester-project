/*
 * CFile1.c
 *
 * Created: 02-11-2018 15:15:28
 *  Author: lars-p
 */ 

#include "ADCTimer_drv.h"

void NodeSetup()
void ADCtimerSetup() //Find new name !!
{
	ADC_setup();
	ADC_input1();
	ADC_pin1();
	//ADC_input2();
	//ADC_input3();
	//ADC_input3();  //Fishy setup. Find out what needs to be in this function 
	
	Timer0_CTC1();
	Timer0_CTC2();
	Timer0_interrupt();
	Timer0_1ms();
<<<<<<< HEAD
	sei();
	
	sensor_at_node sensor1;
	sensor_at_node sensor2;
=======

>>>>>>> Jakob_Groeftehauge
}

