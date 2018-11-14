/*
 * master_HUB_node.c
 *
 * Created: 11/9/2018 3:58:50 PM
 * Author : Jakob
 */ 

#include <avr/io.h>
#include "CAN_drv.h"
#include "HUB_lib.h"
#define NUMBER_OF_SENSOR 3
 
volatile uint8_t tick; 
volatile uint8_t heartBeat; 
volatile uint8_t receivedMessage; 
volatile uint8_t i; 


int main(void)
{
	heartBeat = 0; 

	uint8_t receive_buffer[MSG_SIZE];
	st_cmd_t receiveMOb;
	receiveMOb.pt_data = &receive_buffer[0];
	receiveMOb.MObNumber = 0x01;
	receiveMOb.dlc = MSG_SIZE;
	receiveMOb.cmd = RX;
	receiveMOb.id = 0x0000;


	uint8_t transmit_buffe[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffe[0];
	transmitMOb.MObNumber = 0x02; 
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x0000;  

sensorData sensorList[NUMBER_OF_SENSOR];

//Setup sensorData structs

sensorList[0].sensorStruct.CAN_ID = 0x00FF;
sensorList[0].sensorStruct.sampling_frequency = 0xFF; 
sensorList[0].sensorStruct.filter_parameter = 0xFF;  
sensorList[0].sensorStruct.unit = 0xFF; 
sensorList[0].sensorStruct.filter_type = 0xFF;
sensorList[0].sensorStruct.range_max = 0xFF;
sensorList[0].sensorStruct.range_min = 0xFF;
sensorList[0].data = 0; 
sensorList[0].isSCS = 1; 

sensorList[1].sensorStruct.CAN_ID = 0x00FF;
sensorList[1].sensorStruct.sampling_frequency = 0xFF;
sensorList[1].sensorStruct.filter_parameter = 0xFF;
sensorList[1].sensorStruct.unit = 0xFF;
sensorList[1].sensorStruct.filter_type = 0xFF;
sensorList[1].sensorStruct.range_max = 0xFF;
sensorList[1].sensorStruct.range_min = 0xFF;
sensorList[1].data = 0;
sensorList[1].isSCS = 1;

sensorList[2].sensorStruct.CAN_ID = 0x00FF;
sensorList[2].sensorStruct.sampling_frequency = 0xFF;
sensorList[2].sensorStruct.filter_parameter = 0xFF;
sensorList[2].sensorStruct.unit = 0xFF;
sensorList[2].sensorStruct.filter_type = 0xFF;
sensorList[2].sensorStruct.range_max = 0xFF;
sensorList[2].sensorStruct.range_min = 0xFF;
sensorList[2].data = 0;
sensorList[2].isSCS = 1;

// Add more....
chip_init();
can_init();
initSensors(&sensorList[NUMBER_OF_SENSOR], &transmitMOb);
sei();


if(tick > 0)
{



if(receivedMessage > 0)
{
	updateData(&sensorList, &receiveMOb);

	receivedMessage--; 
}
    
if(heartBeat > 20)
{
	for (i = 0; i < NUMBER_OF_SENSOR; i++)
	{

		if(sensorList[i].isSCS == 1)
		{

			if(sensorList[i].numberOfMessages < 0)
			{
				//Send alert
			}

		}

	}
 
heartBeat = 0; 
}



}

tick--; 
}

void chip_init(void){

	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	bit_set(PORTD, BIT(1));
	//bit_set(PORTD, BIT(7));

}



ISR(TIMER0_COMPA_vect)
{
	tick++;
	heartBeat++; 

}

ISR( CAN_INT_vect )
{
	receivedMessage++;

}