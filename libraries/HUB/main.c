/*
 * master_HUB_node.c
 *
 * Created: 11/9/2018 3:58:50 PM
 * Author : Jakob
 */ 

#define NUMBER_OF_RECEIVEMOBS 5
#include <avr/io.h>
#include "CAN_drv.h"
#include "HUB_lib.h"
#include "payloadProtocol.h"
#include "alertFunction.h"
#include "Timer_drv.h" //test


volatile uint8_t tick; 
volatile uint8_t heartBeatSCS; 
volatile uint8_t heartBeatReg; 
volatile uint8_t receivedMessage; 
volatile uint8_t i; 
st_cmd_t receiveMObs[NUMBER_OF_RECEIVEMOBS];
uint8_t recieveBuffers[NUMBER_OF_RECEIVEMOBS][MSG_SIZE];
volatile sensorData sensorList[NUMBER_OF_SENSOR];
st_cmd_t transmitMOb; 

int main(void)
{

//bit_set(PORTD, BIT(7));

chip_init();
can_init();
TimerSetup();

heartBeatSCS = 0; 


	//st_cmd_t receiveMObs[NUMBER_OF_RECEIVEMOBS];
	//uint8_t recieveBuffers[NUMBER_OF_RECEIVEMOBS][MSG_SIZE];
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

	transmitMOb.pt_data = &transmit_buffer[0];
	transmitMOb.MObNumber = 0x05;
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.id = 0x0000;  


//volatile sensorData sensorList[NUMBER_OF_SENSOR];
floatUnion coefList1[2]; 
floatUnion coefList2[2];
floatUnion coefList3[2];
floatUnion coefList4[2];
floatUnion coefList5[3];


coefList1[0].floatVal = 0;
coefList1[1].floatVal = 1;

coefList2[0].floatVal = 0;
coefList2[1].floatVal = 1;

coefList3[0].floatVal = -12.5;
coefList3[1].floatVal = 25;

coefList4[0].floatVal = 0;
coefList4[1].floatVal = 1;

coefList5[0].floatVal = 0;
coefList5[1].floatVal = 2;
coefList5[2].floatVal = 1;
//Setup sensorData structs

sensorList[0].sensorStruct.CAN_ID = 200;
sensorList[0].sensorStruct.samplingfreq = 0;
sensorList[0].sensorStruct.period = 10;
sensorList[0].sensorStruct.cutOffFreq = 10;
sensorList[0].sensorStruct.unit = other_unit;
sensorList[0].sensorStruct.sensor_Type = potentiometer;
sensorList[0].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[0].sensorStruct.polynomialList = &coefList1[0];
sensorList[0].numberOfErrors = 0;
sensorList[0].ACK = 0;
sensorList[0].data.binVal = 0;
sensorList[0].isSCS = 0;



sensorList[1].sensorStruct.CAN_ID = 202;
sensorList[1].sensorStruct.samplingfreq = 0;
sensorList[1].sensorStruct.period = 10;
sensorList[1].sensorStruct.cutOffFreq = 12;
sensorList[1].sensorStruct.unit = percentage;
sensorList[1].sensorStruct.sensor_Type = potentiometer;
sensorList[1].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[1].sensorStruct.polynomialList = &coefList2[0];
sensorList[1].numberOfErrors = 0;
sensorList[1].data.binVal = 0;
sensorList[1].ACK = 0;
sensorList[1].isSCS = 0;

sensorList[2].sensorStruct.CAN_ID = 100;
sensorList[2].sensorStruct.samplingfreq = 0;
sensorList[2].sensorStruct.period = 5;
sensorList[2].sensorStruct.cutOffFreq = 15;
sensorList[2].sensorStruct.unit = percentage;
sensorList[2].sensorStruct.sensor_Type = potentiometer;
sensorList[2].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[2].sensorStruct.polynomialList = &coefList3[0];
sensorList[2].numberOfErrors = 0;
sensorList[2].data.binVal = 0;
sensorList[2].isSCS = 1;

sensorList[3].sensorStruct.CAN_ID = 204;
sensorList[3].sensorStruct.samplingfreq = 0;
sensorList[3].sensorStruct.period = 10;
sensorList[3].sensorStruct.cutOffFreq = 20;
sensorList[3].sensorStruct.unit = percentage;
sensorList[3].sensorStruct.sensor_Type = potentiometer;
sensorList[3].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[3].sensorStruct.polynomialList = &coefList4[0];
sensorList[3].numberOfErrors = 0;
sensorList[3].data.binVal = 0;
sensorList[3].isSCS = 0;

sensorList[4].sensorStruct.CAN_ID = 206;
sensorList[4].sensorStruct.samplingfreq = 0;
sensorList[4].sensorStruct.period = 10;
sensorList[4].sensorStruct.cutOffFreq = 10;
sensorList[4].sensorStruct.unit = celsius;
sensorList[4].sensorStruct.sensor_Type = thermistor;
sensorList[4].sensorStruct.totalNumberOfpolynomials = 3;
sensorList[4].sensorStruct.polynomialList = &coefList5[0];
sensorList[4].numberOfErrors = 0;
sensorList[4].data.binVal = 0;
sensorList[4].isSCS = 0;

//bit_set(PORTD, BIT(1));
sei(); 


initSensors(sensorList, &transmitMOb);
//setupAlertFunction(&blinkLED());


transmitMOb.id = 0x0000; //reset CAN-id
//receivedMessage = 0; 



while(1)
{
	bit_set(PORTD,BIT(7)); //Test related "Start"
	uint8_t k = 0;
	for(uint8_t i=0;i<NUMBER_OF_SENSOR;i++)
	{
		if(sensorList[i].ACK==1)
		{
			k++;
		}
	}
	if (k==NUMBER_OF_SENSOR)
	{
		bit_clear(PORTD,BIT(7));
	}
	//Test stop
	
// 	if(tick > 0)
// 	{
// 		if(heartBeatSCS > 20)
// 		{
// 			if (heartBeatReg > 5)
// 			{
// 				for(i = 0; i < NUMBER_OF_SENSOR; i++)
// 				{
// 				if(sensorList[i].numberOfMessages ==  0 && sensorList[i].ACK)
// 				{
// 				//alertFunction(); 
// 				}
// 				sensorList[i].numberOfMessages = 0; 
// 				}
// 			heartBeatReg = 0; 
// 			}
// 			else
// 			{
// 				for (i = 0; i < NUMBER_OF_SENSOR; i++)
// 				{
// 					if(sensorList[i].isSCS == 1)
// 					{
// 						if(sensorList[i].numberOfMessages == 0 && sensorList[i].ACK)
// 						{
// 						//alertFunction(); 
// 						}
// 						sensorList[i].numberOfMessages = 0; 
// 					}
// 				}
// 			}
// 			heartBeatReg++; 
// 			heartBeatSCS = 0; 
// 		}
// 	}
// 	heartBeatSCS++; 
// 	tick--; 
}

}


void chip_init(void){

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
	//bit_set(PORTD, BIT(7));
	uint8_t HPMOb = (CANHPMOB & 0xF0) >> 4;
	transfer_data(&receiveMObs[HPMOb]);
	
	switch(receiveMObs[HPMOb].pt_data[0] & 0xFF)
	{
		case 0x30:
		{
			updateData(sensorList, &receiveMObs[HPMOb]); 
			break; 
		}
		case 0xC1://ACK FROM NODE
		{
			ACKnode(sensorList, &receiveMObs[HPMOb]); 
			break; 
		}
		case 0xA0: //ERROR SIGNAL 
		{
			//alertFunction();
			increaseErrorCounter(sensorList, &receiveMObs[HPMOb], &transmitMOb);
			bit_flip(PORTD, BIT(7));
			break;//ERROR SIGNAL
		}
		case 0xC8: // Sensor Requester
		{
			sensorRequesterSetup(sensorList, &receiveMObs[HPMOb],&transmitMOb);
			break;
		}
		default: 
		{
			break; 
		}
		}
}