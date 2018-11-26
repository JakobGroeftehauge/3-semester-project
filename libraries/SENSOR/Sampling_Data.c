/*
 * Sampling_Data.c
 *
 * Created: 14-11-2018 12:55:06
 *  Author: Peter
 */ 
#include "Sampling_Data.h"
#include <avr/interrupt.h>


void sampleData(sensor_at_node* sensor)
{
	float ADC_data=0;
	uint16_t buffer=0;
	static uint8_t preventWireOff = 0; 

	if(preventWireOff < 101)
	{
	preventWireOff++; 
	}
	
	
	if (sensor->sensorNumber == 1) 
	{
		ADC_input1();
		ADC_Start();
		while(ADIF == 0)
		{
			bit_flip(PORTD,BIT(7));
		}
		buffer =ADCL; //shifter med 4
		buffer += ADCH*256;
		ADC_data = buffer;
		bit_set(ADCSRA,BIT(4));
		ADC_data = 10; 
		sensor->filterValue.floatVal = calculateFilterAlternative(ADC_data, sensor->filterPt, &(sensor->bufferList));


	} 
	else if (sensor->sensorNumber==2)
{	
		ADC_input2(); //ADC7
		ADC_Start();
		
		while(ADIF == 0)
		{
			
		}
		buffer =ADCL; //shifter med 4
		buffer += ADCH*256;
		ADC_data = buffer;
		bit_set(ADCSRA,BIT(4));
		ADC_data = 10;
		sensor->filterValue.floatVal = calculateFilterAlternative(ADC_data, sensor->filterPt, &(sensor->bufferList));
	}
	if(preventWireOff > 100)
	{
	Wire_off(ADC_data,sensor);
	}
}

void Wire_off(uint16_t data ,sensor_at_node* sensor)
{
	
	if (data<102 && sensor->period != 0)
	{
		// FUNCTION TRANSMIT ERROR MESSAGE FOR LOW WIRE-OFF DETECTION!!
		sendError(sensor,0b00000001);
	}
	if (data>921  && sensor->period != 0)
	{
		// FUNCTION TRANSMIT ERROR MESSAGE FOR HIGH WIRE-OFF DETECTION!!
		sendError(sensor,0b00000010);
	}
}
