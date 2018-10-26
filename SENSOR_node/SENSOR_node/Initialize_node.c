/*
 * Initialisering.c
 *
 * Created: 24-10-2018 10:55:01
 *  Author: lars-p
 */ 

#include "definition.h"
#include "Initialize_node.h"


void NodeSetup();
{
	ADC_setup();
	
	sei(); 
			
}

void SensorType(uint8_t type_of_sensor, uint8_t MOSFET)
{	
	switch(type_of_sensor)
	{
		case 1: /// thermistor
		{
			if (MOSFET)	// depends on what sensor number there is being used.
											// Sensor is a array of the struct created by the decoder.
			{
				//setup of pins for the MOSFET (NEED TO BE DEFINED)
				MOSFET_Pin();
				
				//Setup of pins for input and ADC
				ADC_pin1();
				
			}
			else
			{
			ADC_pin1();
			}
			break;
		}
		
		case 2: //Potentiometer
		{
			//Setup of pins for input and ADC
				ADC_pin2();
			break;
		}
		
		default : //other_sensor
		{
			//Setup of pins for input and ADC
				ADC_pin3();
			break;
		}
	
	}
	
}

void setTransmissionFrequency(uint8_t Tfrequency) 
{
	switch(Tfrequency)
	{
		case 1:
		{
			//Set timer for specific transmission frequency
			TransmissionType1();
			break;
		}
		
		case 2:
		{
			//Set timer for specific transmission frequency
			TransmissionType2();
			break;
		}
		default
		{
			//Set timer for specific transmission frequency
			TransmissionType3();
			break;
		}
	
	}
}

void SamplingFrequency(uint8_t Sfrequency)
{
switch(Sfrequency)
{
	case 1:
	{
		//Set timer for specific sampling frequency
		SamplingType1();
		break;
	}
	
	case 2:
	{
		//Set timer for specific sampling frequency
		SamplingType2();
		break;
	}
	default
	{
		//Set timer for specific sampling frequency
		SamplingType3();
		break;
	}
	
}

}

void Filter(uint8_t,uint8_t)
{
	
}