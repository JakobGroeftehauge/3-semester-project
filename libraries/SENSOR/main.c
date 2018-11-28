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
#define NUMBER_OF_RECEIVEMOBS 4
#define Sensor1_ID 0x1
#define Sensor2_ID 0x2
#define polynomialSize 8
volatile uint8_t tick = 0; // Used by the timer
volatile uint8_t receivedMessages = 0; 
volatile sensor_at_node Sensorlist[NUMBER_OF_SENSOR];
volatile uint8_t data[MSG_SIZE];
volatile uint8_t receiveBuffers[NUMBER_OF_RECEIVEMOBS][MSG_SIZE];
volatile st_cmd_t receiveMObs[NUMBER_OF_RECEIVEMOBS];
volatile uint8_t transmitBuffers[NUMBER_OF_SENSOR][MSG_SIZE];
volatile st_cmd_t transmitMObs[NUMBER_OF_SENSOR];
volatile float polynomialLists[NUMBER_OF_SENSOR][polynomialSize];
Filter lowPass1 = {6, 10,{{-0.978726409252575,0.251141008333413},{-1.124780222959660,0.415320779783934},{-1.409974885144777,0.749736858010378}},{{1, 1.682614394119966, 1},{1, 0.434137484442287, 1},{0.001650259552247, -0.0003005929665964,0.001650259552249}}};



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

	// --------------------------- setup CAN ID -------------------------------
	Sensorlist[0].CAN_ID = Sensor1_ID;
	Sensorlist[1].CAN_ID = Sensor2_ID;
		
	//setup pointers in sensor structs, and other default values
	for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
		Sensorlist[i].transmissionMOb = &transmitMObs[i];
		Sensorlist[i].receiveMOb = &receiveMObs[i];
		
		Sensorlist[i].polynomialList = &polynomialLists[i];
		Sensorlist[i].totalNumberOfpolynomials = 2;					//Default setting for polynomial value
		Sensorlist[i].polynomialList[1].floatVal = 1;				//Default settings. Will return the value of the filtered data and does not need to be converted with a polynomial at hub.
	}
	
	
	//Setup receive MOb
	
	for (uint8_t i = 0; i < NUMBER_OF_RECEIVEMOBS; i++)
	{
		receiveMObs[i].pt_data = &receiveBuffers[i];
		receiveMObs[i].MObNumber = i;
		receiveMObs[i].dlc = MSG_SIZE;
		receiveMObs[i].cmd = RX;
		receiveMObs[i].mask = 0b11111111;
		receiveMObs[i].newData = 0;
		if (i < NUMBER_OF_RECEIVEMOBS/NUMBER_OF_SENSOR)
		{
			receiveMObs[i].id = Sensorlist[0].CAN_ID;
		}
		else
		{
			receiveMObs[i].id = Sensorlist[1].CAN_ID;
		}
	}

	// ------- More similar to old approach
	//for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	//{
		//receiveMObs[i].pt_data = &receiveBuffers[i];
		//receiveMObs[i].MObNumber = i;
		//receiveMObs[i].dlc = MSG_SIZE;
		//receiveMObs[i].cmd = RX;
		//receiveMObs[i].mask = 0b11111111;
		//receiveMObs[i].id = Sensorlist[i].CAN_ID;
		//receiveMObs[i].newData = 0;
	//}
		//
	//setup transmit MOb
	for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
		transmitMObs[i].pt_data = &transmitBuffers[i];
		transmitMObs[i].MObNumber = NUMBER_OF_RECEIVEMOBS + i;
		transmitMObs[i].dlc = MSG_SIZE;
		transmitMObs[i].cmd = TX;
		transmitMObs[i].id = Sensorlist[i].CAN_ID;
	}
	
	
	//Setup sensor number (used to sample data)
	if (Sensorlist[0].CAN_ID>Sensorlist[1].CAN_ID)	//Determines which sensor should have pin 1 and pin 2  	//The one with lowest CAN ID will have pin 1 as input.										//The one with lowest CAN ID will have pin 1 as input.
	{
		Sensorlist[0].sensorNumber = 1;
		Sensorlist[1].sensorNumber = 2;	
	}
	else
	{
		Sensorlist[1].sensorNumber = 1;
		Sensorlist[0].sensorNumber = 2;
	}

	//-----------------------------SETUP FILTER-------------------------------//
		

			for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
			{
				//assignFilter(&Sensorlist[i], &lowPass1, 1);
				//Sensorlist[i].filterPt = &lowPass1;
				flushBuffers(&(Sensorlist[i].bufferList[0]),6);
			}
	//------------------- end of default settings for sensor ---------------- // 
	
	node_init();			//Setup for pins for output
	can_init(); 
	ADCSetup();					// ADC Drive 
	TimerSetup();				// Timer Drive: Will start a timer with a 1kHz interrupt.
	can_cmd(&receiveMObs[0]);	// Setting up receiveMOb
	can_cmd(&receiveMObs[1]);
	can_cmd(&receiveMObs[2]);	// Setting up receiveMOb
	can_cmd(&receiveMObs[3]);
	bit_set(PORTD,BIT(1));
	sei();					// Global interrupt enable
	
	while(tick<255)
	{	
	}
	
	if(Sensorlist[0].sensorSetupBool == 0)
	{
		sendSensorRequesterSetup(&Sensorlist[0]);
	}
	
	if(Sensorlist[1].sensorSetupBool == 0)
	{
		sendSensorRequesterSetup(&Sensorlist[1]);
	}

	Sensorlist[0].samplingfreq = 100;
	Sensorlist[1].samplingfreq = 100;

//-------------------- MAIN CODE ---------------------------------//
while(1)
{		
	for (uint8_t i = 0; i < NUMBER_OF_RECEIVEMOBS; i++)
	{
		if (receiveMObs[i].newData == 1)
		{
			for (uint8_t u = 0; u < NUMBER_OF_SENSOR; u++)
			{
				if (receiveMObs[i].id == Sensorlist[u].CAN_ID)
				{
					decodeMessage2(&Sensorlist[u], &receiveMObs[i], &lowPass1);
					receiveMObs[i].newData = 0;
				}
			}

		}
			
	}
	
	// ------ More similar to old approach, uncomment if new doesnt work --------------
	//for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	//{
		//if (Sensorlist[i].receiveMOb->newData == 1)
		//{
			//decodeMessage2(&Sensorlist[i], &lowPass1);
			//Sensorlist[i].receiveMOb->newData == 0;
		//}
		//
	//}
	
	if (tick>=1)			// Timer interrupt counter (1ms)
	{
		tick--;				
		samplingCounter1++;	// Sampling counter 1
		samplingCounter2++;	// Sampling counter 2
		transmitCounter1++;	// Transmitting counter 1
		transmitCounter2++;	// Transmitting counter 2
		
//---------------------- Sampling data ------------------- // 		
		if ((samplingCounter1) >= Sensorlist[0].samplingfreq && Sensorlist[0].samplingfreq !=0 )	//Determines if it is time to sample data for sensor 1. 
		{
			sampleData(&Sensorlist[0]);
			//float input0 = 1;
			//Sensorlist[0].filterValue.floatVal = calculateFilterAlternative(Sensorlist[0].filterValue.floatVal, Sensorlist[0].filterPt, &(Sensorlist[0].bufferList));															//Samples the data and filter it. 
			samplingCounter1 = 0;	
			
		}
		if ((samplingCounter2) >= Sensorlist[1].samplingfreq && Sensorlist[1].samplingfreq !=0  )	//Same as above
		{
			sampleData(&Sensorlist[1]);
			//float input1 = 1;
			//Sensorlist[0].filterValue.floatVal = calculateFilterAlternative(Sensorlist[1].filterValue.floatVal, Sensorlist[1].filterPt, &(Sensorlist[1].bufferList));
			samplingCounter2 = 0;
		}
//-------------------- Transmitting data ------------------- // 
		if ((transmitCounter1/2) >= Sensorlist[0].period && Sensorlist[0].period != 0)				//Determines if it is time to transmit data for sensor 1. 
		{
			sendFilteretData(&Sensorlist[0]);//Sending the data. The data have been converted using the polynomial and filtered. 
			transmitCounter1=0;
		}
		if ((transmitCounter2/2) >= Sensorlist[1].period && Sensorlist[1].period != 0)				//Same as above.
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
	uint8_t saveCanpage = CANPAGE;
	//receivedMessages++;			// Inc receivedMessages which will be used in main loop.
	uint8_t HPMOb = (CANHPMOB & 0xF0)>>4;
	
	// New approach, logic in main loop
	transfer_data(&receiveMObs[HPMOb]);
	
	//----------- Similar to old approach --------------
	//transfer_data((Sensorlist[HPMOb].receiveMOb));
	//
	//decodeMessage2(&Sensorlist[HPMOb], &lowPass1);
	CANPAGE = saveCanpage;
}

//ISR(ADC_vect)
//{
	//
//}