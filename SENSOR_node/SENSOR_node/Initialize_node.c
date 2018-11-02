/*
 * Initialisering.c
 *
 * Created: 24-10-2018 10:55:01
 *  Author: lars-p
 */ 

#include "definition.h"
#include "Initialize_node.h"

void NodeSetup()
{
	ADC_setup();
	ADC_input1();
	ADC_input2();
	ADC_input3();
	
	sei(); 
			
}

void SensorType(uint8_t type_of_sensor) //SHOULD NOT BE HERE! 
{	
	switch(type_of_sensor)
	{
		case 1: /// thermistor
		{
					
				//Setup of pins for input and ADC
				ADC_pin1();
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

void setTransmissionFrequency(uint8_t Tfrequency)  // SHOULD NOT BE HERE
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
		default:
		{
			//Set timer for specific transmission frequency
			TransmissionType3();
			break;
		}
	
	}
}

void setSamplingFrequency(uint8_t Sfrequency) // SHOULD NOT BE HERE
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
		default:
		{
			//Set timer for specific sampling frequency
			SamplingType3();
			break;
		}
	
	}

}

void Filter(uint8_t a,uint8_t b) //Should not be here
{
	
}


