/*
 * Initialisering.c
 *
 * Created: 24-10-2018 10:55:01
 *  Author: lars-p
 */ 

#include "definition.h"
#include "Initialize_node.h"
//#include "definition.h"
//#include "Initialize_node.h"


<<<<<<< HEAD
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
	
}}
=======
//void NodeSetup();
//{
	//ADC_setup();
	//
	//sei(); 
	//void (*ADCPIN[2])();
	//ADCPIN[0]=ADC_pin1;
	//ADCPIN[1]=ADC_pin2;
	//ADCPIN[2]=ADC_pin3;
	//
	//void (*timers[2])();
	//timers[0]=TransmissionType1;
	//timers[1]=TransmissionType2;
	//timers[2]=TransmissionType3;
//}
//
//void SensorType(uint8_t type_of_sensor, uint8_t sensor_Number)
//{	
	//switch(type_of_sensor)
	//{
		//case 1: /// thermistor
		//{
			//if (sensor[i].resistor_MOSFET) // depends on what sensor number there is being used.
			//{
				////setup of pins for the MOSFET (NEED TO BE DEFINED)
				//MOSFET_Pin();
				//
				////Setup of pins for input and ADC
				//(*ADCPIN[sensor_Number])();
				//
			//}
			//else
			//{
			////error 
			//}
			//break;
		//}
		//case 2: //Potentiometer
		//{
			////Setup of pins for input and ADC
				//(*ADCPIN[sensor_Number])();
			//break;
		//}
		//default : //other_sensor
		//{
			////Setup of pins for input and ADC
				//(*ADCPIN[sensor_Number])();
			//break;
		//}
	//
	//}
	//
//}
//
//void setTransmissionFrequency(uint8_t k) 
//{
	//switch(k)
	//{
		//case(1):
		//{
		//
		//}
	//
	//}
//}
//
//
//void SamplingFrequency(uint8_t a)
//{
//
//}
>>>>>>> Initialisation_mads
