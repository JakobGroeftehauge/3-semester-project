/*
 * HUB_lib.h
 *
 * Created: 11/13/2018 2:49:00 PM
 *  Author: Jakob
 */ 




#ifndef HUB_LIB_H_
#define HUB_LIB_H_

#define NUMBER_OF_SENSOR 2

#include "payloadProtocol.h"

typedef struct {
	//uint16_t id;
	uint8_t data;
	uint8_t numberOfMessages;
	uint8_t isSCS;
	uint8_t ACK; 
	uint8_t status; 
	uint8_t	numberOfErrors; 
	sensor_at_node sensorStruct; 
} sensorData;


extern void initSensors(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* transmitMOb); 

extern void updateData(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb);

extern void ACKnode(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb); 

extern void sensorRequesterSetup(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb, st_cmd_t* transmitMOb); 

extern void sendServiceMessage(sensor_at_node* sensorStruct, st_cmd_t* transmitMOb);

extern void sendShutdown(st_cmd_t* transmitMOb, sensorData sensor);

extern void increaseErrorCounter(sensorData sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb, st_cmd_t* transmitMOb); 

#endif /* HUB_LIB_H_ */