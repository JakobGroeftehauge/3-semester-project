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

	for(uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
		sendServiceMessage(&sensorNum[i].sensorStruct, transmitMOb);
	}

 }



 void updateData(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t receiveMObs[5])
 {
	uint8_t HPMOb = (CANHPMOB & 0xF0) >> 4; 
	transfer_data(&receiveMObs[HPMOb]); 
	 for(uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i].sensorStruct.CAN_ID == receiveMObs[0].id)
		 {
			bit_flip(PORTD, BIT(1));
			sensorNum[i].data = receiveMObs[HPMOb].pt_data[1];  //Change to support floats
			sensorNum[i].numberOfMessages++;
			return; 		
		 }
	 }


	 

 }