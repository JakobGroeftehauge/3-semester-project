/*
 * HUB_lib.c
 *
 * Created: 11/13/2018 2:48:30 PM
 *  Author: Jakob
 */ 

 //#include "HUB_lib.h"

 //#include "CAN_lib.h"
 #include "CAN_drv.h"
 #include "HUB_lib.h"
 #include "Timer_drv.h"



 void initSensors(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* transmitMOb)
 {
	for(uint8_t u = 0; u < 3; u++)
	{
		for(uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
		{

			if(sensorNum[i].ACK == 0)
			{
				_delay_ms(20);
				sendServiceMessage(&sensorNum[i].sensorStruct, transmitMOb);
			}

		}
	}
 }



 void updateData(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb)
 {
	//uint8_t HPMOb = (CANHPMOB & 0xF0) >> 4; 
	//transfer_data(&receiveMObs[HPMOb]); 
	 for(volatile uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i].sensorStruct.CAN_ID == receiveMOb->id)
		 {
			//bit_flip(PORTD, BIT(1));
			sensorNum[i].data = receiveMOb->pt_data[1];  //Change to support floats
			sensorNum[i].numberOfMessages++;
			return; 		
		 }
	 }
 }

void ACKnode(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb)
 {

 for(volatile uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
 {

	 if(sensorNum[i].sensorStruct.CAN_ID == receiveMOb->id)
	 {
		 sensorNum[i].ACK = 0x01;  //Change to support floats
		 return;
	 }
 }
 
 }
 
 void sensorRequesterSetup(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb, st_cmd_t* transmitMOb)
 {
	 for(volatile uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i].sensorStruct.CAN_ID == receiveMOb->id)
		 {
			 _delay_ms(20);
			 sendServiceMessage(&sensorNum[i].sensorStruct, transmitMOb);
			 return;
		 }
	 }
	 
 }