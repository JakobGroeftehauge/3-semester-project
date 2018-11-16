/*
 * master_HUB_node.c
 *
 * Created: 11/9/2018 3:58:50 PM
 * Author : Jakob
 */ 

#include <avr/io.h>
#include "CAN_drv.h"
#include "HUB_lib.h"
#include  "payloadProtocol.h"
#define NUMBER_OF_SENSOR 3
 
volatile uint8_t tick; 
volatile uint8_t heartBeat; 
volatile uint8_t receivedMessage; 
volatile uint8_t i; 


int main(void)
{
	chip_init();
	can_init();
	TimerSetup();
	
	//bit_flip(PORTD, BIT(7));
	heartBeat = 0; 

	uint8_t receive_buffer[MSG_SIZE];
	st_cmd_t receiveMOb;
	receiveMOb.pt_data = &receive_buffer[0];
	receiveMOb.MObNumber = 0x01;
	receiveMOb.dlc = MSG_SIZE;
	receiveMOb.cmd = RX;
	receiveMOb.id = 0x0000;


	uint8_t transmit_buffer[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffer[0];
	transmitMOb.MObNumber = 0x02;
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x0000;  


volatile sensorData sensorList[NUMBER_OF_SENSOR];
polyCoef coefList1[2]; 
polyCoef coefList2[2];
polyCoef coefList3[2];


coefList1[0].floatCoef = 23.545; 
coefList1[1].floatCoef = 343.214; 

coefList2[0].floatCoef = 23.545;
coefList2[1].floatCoef = 343.214;

coefList3[0].floatCoef = 23.545;
coefList3[1].floatCoef = 343.214;
//Setup sensorData structs

sensorList[0].sensorStruct.CAN_ID = 0x00FF;
sensorList[0].sensorStruct.samplingfreq = 0xFF; 
sensorList[0].sensorStruct.period = 2;
sensorList[0].sensorStruct.cutOffFreq = 2; 
sensorList[0].sensorStruct.unit = celsius; 
sensorList[0].sensorStruct.sensor_Type = other_sensor;
sensorList[0].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[0].sensorStruct.polynomialList = &coefList1[0]; 
sensorList[0].data = 0;
sensorList[0].isSCS = 1; 


sensorList[1].sensorStruct.CAN_ID = 0x00FF;
sensorList[1].sensorStruct.samplingfreq = 2;
sensorList[1].sensorStruct.period = 2; 
sensorList[1].sensorStruct.unit = celsius;
sensorList[1].sensorStruct.sensor_Type = other_sensor;
sensorList[1].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[0].sensorStruct.polynomialList = &coefList2[0]; 
sensorList[1].data = 0;
sensorList[1].isSCS = 1;

sensorList[2].sensorStruct.CAN_ID = 0x00FF;
sensorList[2].sensorStruct.samplingfreq = 0xFF;
sensorList[2].sensorStruct.unit = celsius;
sensorList[2].sensorStruct.sensor_Type = other_sensor;
sensorList[2].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[0].sensorStruct.polynomialList = &coefList3[0]; 
sensorList[2].data = 0;
sensorList[2].isSCS = 1;


initSensors(&sensorList, &transmitMOb);

sei();

while(1)
{

	if(receivedMessage > 0)
	{
		updateData(&sensorList, &receiveMOb);
		receivedMessage--; 
	}

	if(tick > 0)
	{
    
		if(heartBeat > 20)
		{
			for (i = 0; i < NUMBER_OF_SENSOR; i++)
			{

				if(sensorList[i].isSCS == 1)
				{

					if(sensorList[i].numberOfMessages == 0)
					{
						//Send alert
					}

					sensorList[i].numberOfMessages = 0; 
				}

			}
 
			heartBeat = 0; 
		}



	}
	heartBeat++; 
	tick--; 
}

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