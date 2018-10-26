/*
 * IncFile1.h
 *
 * Created: 24-10-2018 10:53:48
 *  Author: lars-p
 */ 

#ifndef INCFILE1_H_
#define INCFILE1_H_

#endif /* INCFILE1_H_ */
//------------------------------------------------------------------------------
//Definitions
uint8_t type_of_sensor;
uint8_t Tfrequency;
uint8_t MOSFET;
uint8_t Sfrequency;

//------------------------------------------------------------------------------

extern uint8_t findByte(uint8_t);						// NOT STARTED

extern void NodeSetup();								// STARTED ON - NOT DONE

extern void SensorType(uint8_t type_of_sensor, uint8_t MOSFET);	// STARTED ON - NOT DONE

 extern void setTransmissionFrequency(uint8_t Tfrequency);		// STARTED ON - NOT DONE

 extern void Filter(uint8_t,uint8_t);					// NOT STARTED

extern void SamplingFrequency(uint8_t Sfrequency);		// STARTED ON - NOT DONE

extern bool checkParameters();							// NOT STARTED

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
typedef  struct{
	uint16_t		CAN_ID;
	uint8_t			sensor_Number;
	sensor_Types    sensor_Type;
	units			unit;	//Will be used to determent the translation of the ADC data.
	bool			resistor_MOSFET;
	uint8_t			transmission_frequency;
	uint16_t		filter_data;
	uint8_t			sampling_frequency;
	
} sensor_at_node;

