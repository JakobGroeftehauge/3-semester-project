/*
 * master_Sensor_Node.c
 *
 * Created: 11/6/2018 12:31:02 PM
 * Author : Jakob
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN_lib.h"
#include "payloadProtocol.h"
#include "Timer_drv.h"
#include "ADC_drv.h"
#include "Sampling_Data.h"
#define NUMBER_OF_SENSOR 2
#define Sensor1_ID 0x1
#define Sensor2_ID 0x2
#define polynomialSize 8
volatile uint8_t tick = 0; // Used by the timer
volatile uint8_t receivedMessages = 0; 
volatile sensor_at_node Sensorlist[NUMBER_OF_SENSOR];
volatile uint8_t data[MSG_SIZE];

//NEEDS TO BE IMPLEMENTED
	//Make the sensor CAN-IDs be based on some kind of offset
	//Maybe allow the CAN-IDs to no be directly after each other.

int main(void)
{
	
	
	//Initialize variable used by main program
	uint8_t samplingCounter1 = 0;
	uint8_t samplingCounter2 = 0;
	uint8_t transmitCounter1 = 0;
	uint8_t transmitCounter2 = 0;


	//Setup receive MOb
	uint8_t receiveBuffers[NUMBER_OF_SENSOR][MSG_SIZE];
	st_cmd_t receiveMObs[NUMBER_OF_SENSOR]; 
	for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
			receiveMObs[i].pt_data = &receiveBuffers[i];
			receiveMObs[i].MObNumber = i;
			receiveMObs[i].dlc = MSG_SIZE;
			receiveMObs[i].cmd = RX;
			receiveMObs[i].mask = 0b11111111;
			receiveMObs[i].id = Sensor1_ID + i;
	}
	

	//Setup transmit MOb  
	uint8_t transmit_buffer[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffer[0]; 
	transmitMOb.MObNumber = 0x03; 
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x00010;  
	
	//-----------------Default settings for sensors--------------------//
	
	//Setup transmit MOb for sensor0
	uint8_t transmit0_buffer[MSG_SIZE];
	st_cmd_t transmitMOb0;
	transmitMOb0.pt_data = &transmit0_buffer[0];
	transmitMOb0.MObNumber = 0x02;
	transmitMOb0.dlc = MSG_SIZE;
	transmitMOb0.cmd = TX;
	transmitMOb0.id = Sensor1_ID;
	Sensorlist[0].CAN_ID =	Sensor1_ID;
	Sensorlist[0].transmissionMOb = &transmitMOb0;
	Sensorlist[0].receiveMOb = &receiveMObs[0];
	
	//Setup transmit MOb for sensor0
	uint8_t transmit1_buffer[MSG_SIZE];
	st_cmd_t transmitMOb1;
	transmitMOb1.pt_data = &transmit1_buffer[0];
	transmitMOb1.MObNumber = 0x03;
	transmitMOb1.dlc = MSG_SIZE;
	transmitMOb1.cmd = TX;
	transmitMOb1.id = Sensor2_ID;
	Sensorlist[1].CAN_ID = Sensor2_ID;
	Sensorlist[1].transmissionMOb = &transmitMOb1;
	Sensorlist[1].receiveMOb = &receiveMObs[1];
	
	//Setup polynomiallist for each sensor;
	float polynomialListe1[polynomialSize];
	Sensorlist[0].polynomialList = &polynomialListe1;
	Sensorlist[0].totalNumberOfpolynomials = 2;					//Default setting for polynomial value
	Sensorlist[0].polynomialList[1].floatCoef = 1;				//Default settings. Will return the value of the filtered data and does not need to be converted with a polynomial at hub.
	
	float polynomialListe2[polynomialSize];
	Sensorlist[1].polynomialList = &polynomialListe2;
	Sensorlist[1].totalNumberOfpolynomials = 2;					//Default setting for polynomial value
	Sensorlist[1].polynomialList[1].floatCoef = 1;				//Default settings. Will return the value of the filtered data and does not need to be converted with a polynomial at hub.
	
	//Setup sensor number (used to sample data)
	if (Sensorlist[0].CAN_ID>Sensorlist[1].CAN_ID)	//Determines which sensor should have pin 1 and pin 2  
													//The one with lowest CAN ID will have pin 1 as input.
	{
		Sensorlist[0].sensorNumber = 1;
		Sensorlist[1].sensorNumber = 2;	
	}
	else
	{
		Sensorlist[1].sensorNumber = 1;
		Sensorlist[0].sensorNumber = 2;
	}
	//------------------- end of default settings for sensor ---------------- // 
	
	node_init();			//Setup for pins for output
	can_init(); 
	ADCSetup();					// ADC Drive 
	TimerSetup();				// Timer Drive: Will start a timer with a 1kHz interrupt.
	can_cmd(&receiveMObs[0]);	// Setting up receiveMOb
	can_cmd(&receiveMObs[1]);
	sei();					// Global interrupt enable

//-------------------- MAIN CODE ---------------------------------//
while(1)
{	
	//This below here should probably be moved to interrupt
	//if (receivedMessages > 0)										// Received Messages interrupt (A message is received and is ready to be read)
	//{
		//Sensorlist[0].samplingfreq = 0xFF;
		//Sensorlist[1].samplingfreq = 0xFF;
		//transfer_data(&receiveMOb);									// Transfer the received data to a struct.
		//decodeMessage(&receiveMOb,&Sensorlist,NUMBER_OF_SENSOR);	// Decoding what kind of message is received and runs the necessary functions.
		//receivedMessages = 0;										
	//}
	
	if (tick>=1)			// Timer interrupt counter (1ms)
	{
		tick--;				
		samplingCounter1++;	// Sampling counter 1
		samplingCounter2++;	// Sampling counter 2
		transmitCounter1++;	// Transmitting counter 1
		transmitCounter2++;	// Transmitting counter 2
		
//---------------------- Sampling data ------------------- // 		
		if (samplingCounter1 >= Sensorlist[0].samplingfreq && Sensorlist[0].samplingfreq !=0 )	//Determines if it is time to sample data for sensor 1. 
		{
			sampleData(&Sensorlist[0]);															//Samples the data and filter it. 
			samplingCounter1 = 0;																//The data will be saved in the sensor struct.
		}
		if (samplingCounter2 >= Sensorlist[1].samplingfreq && Sensorlist[1].samplingfreq !=0  )	//Same as above
		{
			sampleData(&Sensorlist[1]);
			samplingCounter2 = 0;
		}
//-------------------- Transmitting data ------------------- // 
		if (transmitCounter1 >= Sensorlist[0].period && Sensorlist[0].period != 0)				//Determines if it is time to transmit data for sensor 1. 
		{
			sendFilteretData(&Sensorlist[0]);													//Sending the data. The data have been converted using the polynomial and filtered. 
			transmitCounter1=0;
		}
		if (transmitCounter2 >= Sensorlist[1].period && Sensorlist[1].period != 0)				//Same as above.
		{
			sendFilteretData(&Sensorlist[1]);
			transmitCounter2=0;
		}
	}
}
}

void node_init(void){

	// Chip initialization
	DDRC = 0b00000100; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));		//Setup LED
	bit_set(DDRD, BIT(7));		//Setup LED
}

ISR(TIMER0_COMPA_vect)			//Timer interrupt (1kHz)
{
	tick++;						// Inc tick which will be used in main loop.
}

ISR( CAN_INT_vect )				//Receive interrupt
{
	//receivedMessages++;			// Inc receivedMessages which will be used in main loop.
	uint8_t HPMOb = CANHPMOB;
	transfer_data(&(Sensorlist[HPMOb].receiveMOb));
	decodeMessage2(&Sensorlist[HPMOb]);
}

ISR(ADC_vect)
{
	
}