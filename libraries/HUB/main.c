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


coefList1[0].floatVal = 0;
coefList1[1].floatVal = 1;

coefList2[0].floatVal = 0;
coefList2[1].floatVal = 1;

coefList3[0].floatVal = 0;
coefList3[1].floatVal = 1;
//Setup sensorData structs

sensorList[0].sensorStruct.CAN_ID = 0x0001;
sensorList[0].sensorStruct.samplingfreq = 2;
sensorList[0].sensorStruct.period = 100;
sensorList[0].sensorStruct.cutOffFreq = 2;
sensorList[0].sensorStruct.unit = celsius;
sensorList[0].sensorStruct.sensor_Type = other_sensor;
sensorList[0].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[0].sensorStruct.polynomialList = &coefList1[0];
sensorList[0].numberOfErrors = 0;
sensorList[0].ACK = 0;
sensorList[0].data = 0;
sensorList[0].isSCS = 1;



sensorList[1].sensorStruct.CAN_ID = 0x0002;
sensorList[1].sensorStruct.samplingfreq = 2;
sensorList[1].sensorStruct.period = 100;
sensorList[1].sensorStruct.cutOffFreq = 2;
sensorList[1].sensorStruct.unit = degrees;
sensorList[1].sensorStruct.sensor_Type = thermistor;
sensorList[1].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[1].sensorStruct.polynomialList = &coefList2[0];
sensorList[1].numberOfErrors = 0;
sensorList[1].data = 0;
sensorList[1].ACK = 0;
sensorList[1].isSCS = 0;

sensorList[2].sensorStruct.CAN_ID = 0x00CC;
sensorList[2].sensorStruct.samplingfreq = 2;
sensorList[2].sensorStruct.period = 2;
sensorList[2].sensorStruct.cutOffFreq = 2;
sensorList[2].sensorStruct.unit = percentage;
sensorList[2].sensorStruct.sensor_Type = potentiometer;
sensorList[2].sensorStruct.totalNumberOfpolynomials = 2;
sensorList[2].sensorStruct.polynomialList = &coefList3[0];
sensorList[2].numberOfErrors = 0;
sensorList[2].data = 0;
sensorList[2].isSCS = 0;


//bit_set(PORTD, BIT(1));
sei(); 


initSensors(sensorList, &transmitMOb);
//setupAlertFunction(&blinkLED());


transmitMOb.id = 0x0000; //reset CAN-id
//receivedMessage = 0; 



while(1)
{
	if(tick > 0)
	{
		if(heartBeatSCS > 20)
		{
			if (heartBeatReg > 5)
			{
				for(i = 0; i < NUMBER_OF_SENSOR; i++)
				{
				if(sensorList[i].numberOfMessages ==  0 && sensorList[i].ACK)
				{
				bit_set(PORTD, BIT(7));
				}
				sensorList[i].numberOfMessages = 0; 
				}
			heartBeatReg = 0; 
			}
			else
			{
				for (i = 0; i < NUMBER_OF_SENSOR; i++)
				{
					if(sensorList[i].isSCS == 1)
					{
						if(sensorList[i].numberOfMessages == 0 && sensorList[i].ACK)
						{
						bit_set(PORTD, BIT(7));
						}
						sensorList[i].numberOfMessages = 0; 
					}
				}
			}
			heartBeatReg++; 
			heartBeatSCS = 0; 
		}
	}
	heartBeatSCS++; 
	tick--; 
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