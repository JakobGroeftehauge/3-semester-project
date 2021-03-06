/*
 * payloadProtocol.h
 *
 * Created: 26/10/2018 12:18:48
 *  Author: mads-
 */ 


#ifndef PAYLOADPROTOCOL_H_
#define PAYLOADPROTOCOL_H_
#include "CAN_lib.h"
#include "ADCTimer_drv.h"


typedef enum  {
	thermistor,
	potentiometer,
	other_sensor
}sensor_Types;

typedef union {
uint32_t	binCoef;
float		floatCoef;
} polyCoef;

typedef enum uint8_t{
	celsius,
	degrees,
	percentage,
	other_unit
	}units;

typedef  struct {
	uint16_t		CAN_ID;
	sensor_Types    sensor_Type;
	units			unit;	//Will be used to determent the translation of the ADC data.
	uint8_t			period;
	uint8_t			cutOffFreq;
	uint32_t		filterValue;
	st_cmd_t*		transmissionMOb;
	uint8_t			samplingfreq;
	polyCoef*		polynomialList;
	uint8_t			totalNumberOfpolynomials;
}sensor_at_node ;

//sensor_at_node Sensor1;		//Holdes the data of each sensor used in a struct
//sensor_at_node Sensor2;

extern void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node* sensor);

extern void sendServiceMessage(sensor_at_node* sensorStruct, st_cmd_t* transmitMOb);//uint8_t type, uint8_t unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par);

extern void decodeMessage(st_cmd_t* message_struct);

#endif /* PAYLOADPROTOCOL_H_ */