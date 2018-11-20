/*
 * payloadProtocol.h
 *
 * Created: 26/10/2018 12:18:48
 *  Author: mads-
 */ 


#ifndef PAYLOADPROTOCOL_H_
#define PAYLOADPROTOCOL_H_
#include "CAN_lib.h"



typedef enum{
	thermistor,
	potentiometer,
	other_sensor
}sensor_Types;

typedef enum {
	celsius,
	degrees,
	percentage,
	other_unit
}units;

typedef union {
	uint32_t	binCoef;
	float		floatCoef;
} polyCoef;

typedef  struct {
	uint16_t		CAN_ID;
	sensor_Types    sensor_Type;
	units			unit;	//Will be used to determent the translation of the ADC data.
	uint8_t			period;
	uint8_t			cutOffFreq;
	float			filterValue;
	st_cmd_t*		transmissionMOb;
	st_cmd_t*		receiveMOb;
	uint8_t			samplingfreq;
	polyCoef*		polynomialList;
	uint8_t			totalNumberOfpolynomials;
	uint8_t			sensorNumber;
}sensor_at_node ;

extern void decodeCoefficient(sensor_at_node* sensor);

extern void sendError(sensor_at_node* Sensor, uint8_t errorType);

extern void checkParameters(sensor_at_node* Sensor);			//NOT DONE

extern void	ACK_TO_Hub(sensor_at_node* Sensor);

extern void decodeHubServiceMessage(sensor_at_node* sensor);

extern void sendServiceMessage(sensor_at_node* sensorStruct, st_cmd_t* transmitMOb);

extern void decodeMessage(st_cmd_t* message_struct,sensor_at_node* SensorList,uint8_t);

extern void decodeMessage2(sensor_at_node* sensor);

extern float runPolynomial(sensor_at_node* sensor);

extern void sendFilteretData(sensor_at_node* Sensor);

extern void shutDownSensor(sensor_at_node* sensor);

#endif /* PAYLOADPROTOCOL_H_ */