/*
 * master_HUB_node.c
 *
 * Created: 11/9/2018 3:58:50 PM
 * Author : Jakob
 */ 

#include <avr/io.h>
#include "CAN_drv.h"
#define NUMBER_OF_SENSOR 4
 
volatile uint8_t tick; 
volatile uint8_t heartBeat; 
volatile uint8_t receivedMessage; 
volatile uint8_t i; 

typedef struct {
uint16_t id; 
uint8_t data; 
uint8_t numberOfMessages; 
uint8_t isSCS; 
} sensorData;

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

sensorData sensorList[NUMBER_OF_SENSOR];

//Setup sensorData structs

sensorList[0].id = 0x00FF;
sensorList[0].data = 0; 
sensorList[0].isSCS = 1; 

// Add more....

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


void updateData(sensorData* sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb)
{
for(i = 0; i < NUMBER_OF_SENSOR; i++)
{

	if(sensorNum[i]->id == receiveMOb->id)
	{
	sensorNum[i]->data = receiveMOb->pt_data;  //Change to support floats
	sensorNum[i]->numberOfMessages++;
	return; 
	}
}

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