/*
 * IncFile1.h
 *
 * Created: 24-10-2018 10:53:48
 *  Author: lars-p
 */ 

#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <stdint.h>

//------------------------------------------------------------------------------
//Definitions
//
//------------------------------------------------------------------------------
extern uint8_t findByte(uint8_t);						// NOT STARTED

extern void NodeSetup();								// STARTED ON - NOT DONE

extern void SensorType(uint8_t, uint8_t);				// STARTED ON - NOT DONE

extern void setTransmissionFrequency(uint8_t);			// NOT STARTED

extern void Filter(uint8_t,uint8_t);					// NOT STARTED

extern void SamplingFrequency(uint8_t);					// NOT STARTED

//extern bool checkParameters();							// NOT STARTED

extern void sendReply();								// NOT STARTED
//test

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

// Struct
typedef  struct {
	uint16_t		CAN_ID;
	uint16_t		sensor_Number;
	sensor_Types    sensor_Type;
	int8_t			range_min;
	int8_t			range_max;
	units			unit;	//Will be used to determent the translation of the ADC data.
	uint8_t			transmission_frequency;
	uint16_t		filter_data;
	uint8_t			sampling_frequency;
	
}sensor_at_node ;

#endif /* INCFILE1_H_ */


