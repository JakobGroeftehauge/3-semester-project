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

typedef  struct {
	uint16_t		CAN_ID;
	sensor_Types    sensor_Type;
	units			unit;	//Will be used to determent the translation of the ADC data.
	uint8_t			period;
	uint8_t			cutOffFreq;
	float			filterValue;
	st_cmd_t*		transmissionMOb;
	uint8_t			samplingfreq;
	float*			polynomialList;
	uint8_t			totalNumberOfpolynomials;
}sensor_at_node ;

extern void decodeCoefficient(sensor_at_node* SensorList,uint8_t message_array[8]);

extern void	ACK_TO_Hub(sensor_at_node* Sensor);

extern void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node* sensor);

extern void sendServiceMessage(uint8_t type, uint8_t unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par);

extern void decodeMessage(st_cmd_t* message_struct,sensor_at_node* SensorList,uint8_t);

extern float runPolynomial(sensor_at_node* sensor);

extern void sendFilteretData(sensor_at_node* Sensor);

extern void sendError(sensor_at_node* Sensor, uint8_t errorType);
#endif /* PAYLOADPROTOCOL_H_ */