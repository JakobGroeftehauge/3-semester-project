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
#define NUMBER_OF_SENSOR 2
#define Sensor1_ID 0x1
#define Sensor2_ID 0x2
#define polynomialSize 8
volatile uint8_t tick = 0; // Used by the timer
volatile uint8_t receivedMessages = 0; 
volatile sensor_at_node Sensorlist[NUMBER_OF_SENSOR];
volatile uint8_t data[MSG_SIZE];
//void noid_init (void);
int main(void)
{
	
	
//Initialize variable used by main program
	uint8_t samplingCounter1 = 0;
	uint8_t samplingCounter2 = 0;
	uint8_t transmitCounter1 = 0;
	uint8_t transmitCounter2 = 0;
	


	//Setup recieve MOb
	uint8_t recieve_buffer[MSG_SIZE];
	st_cmd_t recieveMOb; 
	recieveMOb.pt_data = &recieve_buffer[0];
	recieveMOb.MObNumber = 0x00; 
	recieveMOb.dlc = MSG_SIZE; 
	recieveMOb.cmd = RX; 
	recieveMOb.mask = 0x0000;
	
	//Setup transmit MOb  
	uint8_t transmit_buffer[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffer[0]; 
	transmitMOb.MObNumber = 0x03; 
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x00010;  
	
	//Setup transmit MOb for sensor0
	uint8_t transmit0_buffer[MSG_SIZE];
	st_cmd_t transmitMOb0;
	transmitMOb0.pt_data = &transmit_buffer[0];
	transmitMOb0.MObNumber = 0x01;
	transmitMOb0.dlc = MSG_SIZE;
	transmitMOb0.cmd = TX;
	transmitMOb0.id = Sensor1_ID;
	Sensorlist[0].CAN_ID =	Sensor1_ID;
	Sensorlist[0].transmissionMOb = &transmitMOb0;
	
	//Setup transmit MOb for sensor0
	uint8_t transmit1_buffer[MSG_SIZE];
	st_cmd_t transmitMOb1;
	transmitMOb1.pt_data = &transmit_buffer[0];
	transmitMOb1.MObNumber = 0x02;
	transmitMOb1.dlc = MSG_SIZE;
	transmitMOb1.cmd = TX;
	transmitMOb1.id = Sensor2_ID;
	Sensorlist[1].CAN_ID = Sensor2_ID;
	Sensorlist[1].transmissionMOb = &transmitMOb1;
	
	//Setup polynomiallist for each sensor;
	float polynomialListe1[polynomialSize];
	Sensorlist[0].polynomialList = &polynomialListe1;
	Sensorlist[0].totalNumberOfpolynomials = polynomialSize;
	
	
	float polynomialListe2[polynomialSize];
	Sensorlist[1].polynomialList = &polynomialListe2;
	Sensorlist[1].totalNumberOfpolynomials = polynomialSize;
	
	node_init();			//Setup for pins for output
	can_init(); 
	ADCSetup();				// ADC Drive 
	TimerSetup();			// Timer Drive
	can_cmd(&recieveMOb);	// Setting up recieveMOb
	sei();					// Global interrupt enable


while(1)
{
	if (receivedMessages > 0)		// Received Messages interrupt (A message is received and is ready to be read)
	{
		Sensorlist[0].filterValue=10;
		Sensorlist[1].filterValue=10;
		transfer_data(&recieveMOb);	// Transfer the received data to rec
		decodeMessage(&recieveMOb,&Sensorlist,NUMBER_OF_SENSOR);
		receivedMessages = 0;
		bit_set(PORTD,BIT(7));
	}
	
	if (tick>=1)			// Timer interrupt counter (1ms)
	{
		tick--;				
		samplingCounter1++;	// Sampling counter 1
		samplingCounter2++;	// Sampling counter 2
		transmitCounter1++;	// Transmitting counter 1
		transmitCounter2++;	// Transmitting counter 2

	/*	
		if (samplingCounter1 >= Sensorlist[0].sampling_frequency && Sensorlist[0].sampling_frequency !=0 )
		{
			bit_flip(PORTD, BIT(7));
			samplingCounter1 = 0;
			//Filter(ADC value); //Filter the data
			
		}
		if (samplingCounter2 >= Sensorlist[1].sampling_frequency && Sensorlist[1].sampling_frequency !=0  )
		{
			bit_flip(PORTD, BIT(1));
			samplingCounter2 = 0;
			//Filter(ADC value); //Filter the data
		}
		*/
		
		if (transmitCounter1 >= Sensorlist[0].period && Sensorlist[0].period != 0)
		{
			sendFilteretData(&Sensorlist[0]);
			transmitCounter1=0;
		}
		if (transmitCounter2 >= Sensorlist[1].period && Sensorlist[1].period != 0)
		{
			sendFilteretData(&Sensorlist[1]);
			transmitCounter2=0;
		}
	}
}
}



void node_init(void){

	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
}

ISR(TIMER0_COMPA_vect)
{
	tick++;
}

ISR( CAN_INT_vect )
{
	receivedMessages++; 
	bit_flip(PORTD,BIT(1));
}

