/*
 * Initialisering.c
 *
 * Created: 24-10-2018 10:55:01
 *  Author: lars-p
 */ 

#include "Initialize_node.h"
#include "definition.h"

void setSensorType(uint8_t type_of_sensor)
{
	switch(type_of_sensor)
	{
		case 0: /// thermistor
		{
			if (sensor[i].resistor_MOSFET) // depends on whet sensor number there is being used.
			{
				//setup of pins for the MOSFET
				//Setup of pins for input and ADC
			}
			else
			{
			error 
			}
			break;
		}
		case 1: //Potentiometer
		{
			//Setup of pins for input and ADC
			break;
		}
		default : //other_sensor
		{
			//Setup of pins for input and ADC
			break;
		}
	
	}
	
}

void setSamplingFrequency(uint8_t a)
