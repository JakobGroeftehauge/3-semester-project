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



 void initSensors(sensorData* sensorNum[NUMBER_OF_SENSOR], st_cmd_t* transmitMOb)
 {
	for(uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
		sendServiceMessage(&sensorNum[i]->sensorStruct, &transmitMOb);
	}

 }



 void updateData(sensorData* sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb)
 {
	 for(uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i]->sensorStruct.CAN_ID == receiveMOb->id)
		 {
			 sensorNum[i]->data = receiveMOb->pt_data;  //Change to support floats
			 sensorNum[i]->numberOfMessages++;
			 return;
		 }
	 }

 }