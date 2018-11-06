/*
 * main2.c
 *
 * Created: 31/10/2018 16:07:23
 *  Author: mads-
 */ 
#include "avr/io.h"
#include "avr/interrupt.h"
#include "can_lib.h"
#include "util/delay.h""
#include "ADCTimer_drv.h"
volatile uint8_t tick;
int main2(void)
{
	NodeSetup(); //Metode to init ADC2 PIN 
	
	uint8_t receivedMessages;
	uint8_t samplingCounter;
	uint8_t samplingLimit;
	uint8_t transmitCounter;
	uint8_t transmitLimit;
	uint16_t sensorValue;
	if (tick>=1)
	{	
		tick--;
		samplingCounter++;
		transmitCounter++;
		
		while (receivedMessages>0)
		{
			//decodeMessage(); // Not yet defined
		}
		if (samplingCounter>=samplingLimit)
		{
			samplingCounter=0;
			sensorValue=ADCH;		// here should be a readADC
			
		}
		
		
		if (transmitCounter>=transmitLimit)
		{
			transmitCounter=0;
			//clear buffer
			//load wanted buffer
			//set ID
			// call can_cmd
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	tick++;
}