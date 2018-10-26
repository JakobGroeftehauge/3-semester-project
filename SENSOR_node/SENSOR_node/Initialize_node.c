/*
 * Initialisering.c
 *
 * Created: 24-10-2018 10:55:01
 *  Author: lars-p
 */ 

#include "Initialize_node.h"
#include "definition.h"

void NodeSetup();
{
	ADC_setup();
	
	sei(); 
}

void SensorType(uint8_t type_of_sensor, uint8_t sensor_Number)
{	
	ADCPIN[]={ADC_pin1(), ADC_pin2(),ADC_pin3()}

	switch(type_of_sensor)
	{
		case 0: /// thermistor
		{
			if (sensor[i].resistor_MOSFET) // depends on what sensor number there is being used.
			{
				//setup of pins for the MOSFET (NEED TO BE DEFINED)
				MOSFET_Pin();
				
				//Setup of pins for input and ADC
				ADCPIN[sensor_Number]
				
			}
			else
			{
			//error 
			}
			break;
		}
		case 1: //Potentiometer
		{
			//Setup of pins for input and ADC
			ADCPIN[sensor_Number];
			break;
		}
		default : //other_sensor
		{
			//Setup of pins for input and ADC
			ADCPIN[sensor_Number];
			break;
		}
	
	}
	
}

void SamplingFrequency(uint8_t a)
{

}
