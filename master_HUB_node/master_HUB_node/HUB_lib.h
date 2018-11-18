/*
 * HUB_lib.h
 *
 * Created: 11/13/2018 2:49:00 PM
 *  Author: Jakob
 */ 




#ifndef HUB_LIB_H_
#define HUB_LIB_H_


#include "payloadProtocol.h"
#define NUMBER_OF_SENSOR 3
//#include "HUB_lib.c"

typedef struct {
	//uint16_t id;
	uint8_t data;
	uint8_t numberOfMessages;
	uint8_t isSCS;
	sensor_at_node sensorStruct; 
} sensorData;


extern void initSensors(sensorData* sensorNum[NUMBER_OF_SENSOR], st_cmd_t* transmitMOb); 

extern void updateData(sensorData* sensorNum[NUMBER_OF_SENSOR], st_cmd_t* receiveMOb);

#endif /* HUB_LIB_H_ */