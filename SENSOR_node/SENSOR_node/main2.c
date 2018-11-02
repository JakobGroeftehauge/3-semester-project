/*
 * main2.c
 *
 * Created: 31/10/2018 16:07:23
 *  Author: mads-
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can_lib.h"
#include <util/delay.h>

int main2(void)
{
	uint8_t tick;
	uint8_t receivedMessages;
	uint8_t samplingCounter;
	uint8_t samplingLimit;
	uint8_t transmitCounter;
	uint8_t transmitLimit;
	uint16_t sensorValue;
	//Init timer etc.
	if (tick>=1)
	{
		samplingCounter++;
		transmitCounter++;
		
		while (receivedMessages>0)
		{
			//decodeMessage(); // Not yet defined
		}
		if (samplingCounter>=samplingLimit)
		{
			samplingCounter=0;
			sensorValue=340;		// here should be a readADC
			
		}
		
		if (transmitCounter>=transmitLimit)
		{
			transmitCounter=0
			//clear buffer
			//load wanted buffer
			//set ID
			// call can_cmd
		}
	}
	
	
	
}