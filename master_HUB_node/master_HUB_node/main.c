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
#define NUMBER_OF_RECEIVEMOBS 5
 
volatile uint8_t tick; 
volatile uint8_t heartBeat; 
volatile uint8_t receivedMessage; 
volatile uint8_t i; 


int main(void)
{


chip_init();
can_init();
TimerSetup();

heartBeat = 0; 


	st_cmd_t receiveMObs[NUMBER_OF_RECEIVEMOBS];
	uint8_t recieveBuffers[NUMBER_OF_RECEIVEMOBS][MSG_SIZE];
	for (i = 0; i < NUMBER_OF_RECEIVEMOBS; i++)
	{
		receiveMObs[i].cmd = RX;
		receiveMObs[i].mask = 0x0000;
		receiveMObs[i].MObNumber = i;
		receiveMObs[i].id = 0x0000; 
		receiveMObs[i].dlc = MSG_SIZE; 
		receiveMObs[i].pt_data = &recieveBuffers[i];
		can_cmd(&receiveMObs[i]);
	
	}

	//Setup recieve MOb
	//uint8_t recieve_buffer[MSG_SIZE];
	//st_cmd_t recieveMOb;
	//recieveMOb.pt_data = &recieve_buffer[0];
	//recieveMOb.MObNumber = 0x01;
	//recieveMOb.dlc = MSG_SIZE;
	//recieveMOb.cmd = RX;
	//recieveMOb.mask = 0x0000; 
	//recieveMOb.id = 0x0000;
//
	//can_cmd(&recieveMOb);

	uint8_t transmit_buffer[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffer[0];
	transmitMOb.MObNumber = 0x05;
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x0000;  


volatile sensorData sensorList[NUMBER_OF_SENSOR];
polyCoef coefList1[2]; 
polyCoef coefList2[2];
polyCoef coefList3[2];


coefList1[0].floatCoef = 23.545; 
coefList1[1].floatCoef = 343.214; 

coefList2[0].floatCoef = 19.545;
coefList2[1].floatCoef = 343.214;

coefList3[0].floatCoef = 300.545;
coefList3[1].floatCoef = 343.214;
//Setup sensorData structs

sensorList[0].sensorStruct.CAN_ID = 0x0040;
sensorList[0].sensorStruct.samplingfreq = 2; 
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
sensorList[1].sensorStruct.cutOffFreq = 2; 
sensorList[1].sensorStruct.unit = degrees;
sensorList[1].sensorStruct.sensor_Type = thermistor;
sensorList[1].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[1].sensorStruct.polynomialList = &coefList2[0]; 
sensorList[1].data = 0;
sensorList[1].isSCS = 1;

sensorList[2].sensorStruct.CAN_ID = 0x00CC;
sensorList[2].sensorStruct.samplingfreq = 2;
sensorList[2].sensorStruct.period = 2; 
sensorList[2].sensorStruct.cutOffFreq = 2; 
sensorList[2].sensorStruct.unit = percentage;
sensorList[2].sensorStruct.sensor_Type = potentiometer;
sensorList[2].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[2].sensorStruct.polynomialList = &coefList3[0]; 
sensorList[2].data = 0;
sensorList[2].isSCS = 1;

initSensors(sensorList, &transmitMOb);

transmitMOb.id = 0x0000; //reset CAN-id

sei();

while(1)
{

	if(receivedMessage > 0)
	{
		updateData(sensorList, receiveMObs);
		receivedMessage = 0; 


		for (uint8_t i = 0; i < 8; i++)
		{
		transmitMOb.pt_data[i] = 0x00; 
		}
		transmitMOb.pt_data[0] = sensorList[0].data & 0xFF;
		transmitMOb.pt_data[1] = sensorList[1].data & 0xFF;
		transmitMOb.pt_data[2] = sensorList[2].data & 0xFF; 
		can_cmd(&transmitMOb);
	}

	if(tick > 0)
	{
    
		if(heartBeat > 20)
		{
			
			//transmitMOb.pt_data[0] = sensorList[0].sensorStruct.totalNumberOfpolynomials; 
			//can_cmd(&transmitMOb);

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

}

ISR( CAN_INT_vect )
{
	receivedMessage++;
	bit_set(PORTD, BIT(7));
	bit_set(PORTD, BIT(1));

}