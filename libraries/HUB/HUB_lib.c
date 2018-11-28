/*
 * HUB_lib.c
 *
 * Created: 11/13/2018 2:48:30 PM
 *  Author: Jakob
 */ 
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
	 for(volatile uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i].sensorStruct.CAN_ID == receiveMOb->id)
		 {
			sensorNum[i].data.binVal = (((((receiveMOb->pt_data[2] << 8) + receiveMOb->pt_data[3]) << 8)+receiveMOb->pt_data[4]) << 8)+receiveMOb->pt_data[5];  //Change to support floats
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
		 sensorNum[i].ACK = 0x01;  
		 sensorNum[i].numberOfErrors = 0; 
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
 
 // sendServiceMessage puts parameters into array, which can be sent
 //sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
 void sendServiceMessage(sensor_at_node* sensorAtNode, st_cmd_t* transmitMOb)
 {

	 for (uint8_t i = 0; i < sensorAtNode->totalNumberOfpolynomials; i++)
	 {
		 transmitMOb->pt_data[0] = 0b11000101;
		 transmitMOb->pt_data[1] = ((i) << 4) | sensorAtNode->totalNumberOfpolynomials;
		 transmitMOb->pt_data[5] = sensorAtNode->polynomialList[i].binVal & 0xFF;
		 transmitMOb->pt_data[4] = sensorAtNode->polynomialList[i].binVal >> 8 & 0xFF;
		 transmitMOb->pt_data[3] = sensorAtNode->polynomialList[i].binVal >> 16 & 0xFF;
		 transmitMOb->pt_data[2] = sensorAtNode->polynomialList[i].binVal >> 24 & 0xFF;
		 transmitMOb->pt_data[6] = 0;
		 transmitMOb->pt_data[7] = 0;
		 transmitMOb->id = sensorAtNode->CAN_ID;
		 
		 can_cmd(transmitMOb);
		 _delay_ms(20);
	 }

	 transmitMOb->pt_data[0] = 0b11000011;
	 transmitMOb->pt_data[1] = sensorAtNode->sensor_Type << 4 | sensorAtNode->unit;
	 transmitMOb->pt_data[2] = sensorAtNode->period;
	 transmitMOb->pt_data[3] = sensorAtNode->cutOffFreq;
	 
	 for (uint8_t i = 4; i < 8; i++)
	 {
		 transmitMOb->pt_data[i] = 0x00;
	 }
	 can_cmd(transmitMOb); //send last message
 }

 void increaseErrorCounter(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb, st_cmd_t* transmitMOb)
 {

	 for(volatile uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	 {

		 if(sensorNum[i].sensorStruct.CAN_ID == receiveMOb->id)
		 {
			 sensorNum[i].numberOfErrors++; 
			if(sensorNum[i].numberOfErrors >= 4)
			{
			sendShutdown(transmitMOb, sensorNum[i]); 
			}			
			 return;
		 }
	 }
	 
 }


 void sendShutdown(st_cmd_t* transmitMOb, sensorData sensor)
 {
 transmitMOb->pt_data[0] = 0xC4; 
 transmitMOb->id = sensor.sensorStruct.CAN_ID;
 can_cmd(transmitMOb); 
 }