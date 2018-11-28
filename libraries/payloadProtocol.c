/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"
#include <math.h>



void decodeCoefficient(sensor_at_node* Sensor, st_cmd_t* receiveMOb)
{	
	uint8_t coeffNumber =(receiveMOb->pt_data[1]& 0xF0)/16;

	Sensor->totalNumberOfpolynomials = receiveMOb->pt_data[1]&0b00001111;
	Sensor->polynomialList[coeffNumber].binVal = receiveMOb->pt_data[2];
	Sensor->polynomialList[coeffNumber].binVal = ((Sensor->polynomialList[coeffNumber].binVal) << 8  ) + receiveMOb->pt_data[3];
	Sensor->polynomialList[coeffNumber].binVal = ((Sensor->polynomialList[coeffNumber].binVal) << 8 ) + receiveMOb->pt_data[4];
	Sensor->polynomialList[coeffNumber].binVal = ((Sensor->polynomialList[coeffNumber].binVal) << 8 ) + receiveMOb->pt_data[5];

}

void ACK_TO_Hub(sensor_at_node* Sensor)			// Takes data from the struct and sends it back to the hub. The hub should then be able to checkon it.
{
	Sensor ->sensorSetupBool = 1; //Indicators that the sensor have received and ACK the setup message.
	
	Sensor ->transmissionMOb->pt_data[0] = 0b11000001;
	Sensor ->transmissionMOb->pt_data[1] = (Sensor->sensor_Type)*16+ Sensor->unit;
	Sensor ->transmissionMOb->pt_data[2] = Sensor->period;
	Sensor ->transmissionMOb->pt_data[3] = Sensor->cutOffFreq;
	Sensor ->transmissionMOb->pt_data[4] = Sensor->samplingfreq;
	Sensor ->transmissionMOb->pt_data[5] = Sensor->totalNumberOfpolynomials;
	Sensor ->transmissionMOb->pt_data[6] = 0;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	
	can_cmd(Sensor->transmissionMOb);
}

// Sending an error message with a specific error type defined elsewhere.
void sendError(sensor_at_node* Sensor,uint8_t errorType) 
{
	Sensor ->transmissionMOb->pt_data[0] = 0b10100000;
	Sensor ->transmissionMOb->pt_data[1] = errorType;
	Sensor ->transmissionMOb->pt_data[2] = 0;
	Sensor ->transmissionMOb->pt_data[3] = 0;
	Sensor ->transmissionMOb->pt_data[4] = 0;
	Sensor ->transmissionMOb->pt_data[5] = 0;
	Sensor ->transmissionMOb->pt_data[6] = 0;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	can_cmd(Sensor->transmissionMOb);
}

void checkParameters(sensor_at_node* Sensor)
{

	if(Sensor->totalNumberOfpolynomials>8 || Sensor->totalNumberOfpolynomials == 0)
	{
		sendError(Sensor,0b00000111);
		shutDownSensor(Sensor);
	}
// 	else if (Sensor->period<Sensor->samplingfreq)	// If the sampling freq is lower than the transmission freq an error occures
// 	{
// 		sendError(Sensor,0b00000111);
// 		shutDownSensor(Sensor);
// 	}
// 	else if(Sensor->samplingfreq == 0)				//If the sampling freq == 0 then the desired cutoff freq is not possible
// 	{
// 		sendError(Sensor,0b00000111);
//		shutDownSensor(Sensor);
//	}
	else
	{
		ACK_TO_Hub(Sensor);
	}
	
}

float runPolynomial(sensor_at_node* sensor)
{	
	float result = sensor-> polynomialList[0].floatVal;
	float filterValue = (sensor->filterValue.floatVal/1023)*5; // Transform it from being a value between 0 - 1023 to 0v - 5v
	
	for (uint8_t i=0; i<sensor->totalNumberOfpolynomials-1;i++)
	{
		result = result+ sensor->polynomialList[i+1].floatVal*pow(filterValue,i+1);  
	}
	
	return result;
}

// Sends the filtered data from the sensors given as parameters. Data comes from the struct and will be updated by another function.
void sendFilteretData(sensor_at_node* Sensor)	
{
	floatUnion polynomialValue;
	polynomialValue.floatVal =runPolynomial(Sensor);
	Sensor->transmissionMOb->pt_data[0] = 0x30; // Data message
	Sensor->transmissionMOb->pt_data[1] = (Sensor->sensor_Type*16)+Sensor->unit;
	
	Sensor->transmissionMOb->pt_data[5] = polynomialValue.binVal & 0xFF;
	Sensor->transmissionMOb->pt_data[4] = polynomialValue.binVal >> 8 & 0xFF;
	Sensor->transmissionMOb->pt_data[3] = polynomialValue.binVal >> 16 & 0xFF;
	Sensor->transmissionMOb->pt_data[2] = polynomialValue.binVal >> 24 & 0xFF;

	Sensor ->transmissionMOb->pt_data[6] = 0x00;
	Sensor ->transmissionMOb->pt_data[7] = 0x00;
	can_cmd(Sensor->transmissionMOb);
}

// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(sensor_at_node* sensor, st_cmd_t* receiveMOb)
{
	sensor->sensor_Type = (receiveMOb->pt_data[1] & 0b11110000)/16; // Shift left nibble to the right with /16
	sensor->unit = receiveMOb->pt_data[1] & 0b00001111;
	sensor->period = receiveMOb->pt_data[2];
	sensor->cutOffFreq = receiveMOb->pt_data[3];
}

// shutDownSensor will shut down the node and set default values in struct
void shutDownSensor(sensor_at_node* sensor)
{
	sensor->period = 0;
	sensor->sensor_Type = 0;
	sensor->unit = 0;
	sensor->samplingfreq = 0;
	sensor->filterValue.floatVal = 0;
	sensor->receiveMOb = 0;
	sensor->polynomialList[0].floatVal = 0;
	sensor->polynomialList[1].floatVal = 1;
	sensor->totalNumberOfpolynomials = 2;
	sensor->cutOffFreq = 0;
	sensor ->sensorSetupBool = 0;
	sensor ->filterPt = 0;
}

//Decoding message from hub and determines what kind of message type it is.

void decodeMessage2(sensor_at_node* sensor, st_cmd_t* receiveMOb, Filter* filter) //
{
		
	switch (receiveMOb->pt_data[0])		
	{
		case 0b11000101: //ID for setup of Coefficients
		{
				decodeCoefficient(sensor, receiveMOb);
				break;
		}
		case 0b11000011: // ID FOR A SERVICE MESSAGE
		{
				decodeHubServiceMessage(sensor, receiveMOb);
				assignFilter(sensor, filter, 1);
				checkParameters(sensor);
				break;
		}
		case 0b11100000: // ID FOR DATA RETRIEVING MESSAGE, FROM SPECIFIC MESSAGE
		{
				sendFilteretData(sensor);
				break;
		}
		case 0b11000100: // ID FOR SHUTDOWN SPECFIC SENSOR
		{
				shutDownSensor(sensor);		
				break;
		}
		default:
		{
				break;
		}
		
	}
}

void sendSensorRequesterSetup(sensor_at_node* Sensor)
{
	Sensor ->transmissionMOb->pt_data[0] = 0b11001000;
	Sensor ->transmissionMOb->pt_data[1] = 0;
	Sensor ->transmissionMOb->pt_data[2] = 0;
	Sensor ->transmissionMOb->pt_data[3] = 0;
	Sensor ->transmissionMOb->pt_data[4] = 0;
	Sensor ->transmissionMOb->pt_data[5] = 0;
	Sensor ->transmissionMOb->pt_data[6] = 0;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	can_cmd(Sensor->transmissionMOb);
}

void assignFilter(sensor_at_node* sensor, Filter* filterlist, uint8_t antalFiltre) //ATM there is only one filter. antalFilter is for futur use
{
	
	sensor->filterPt = filterlist;

	uint16_t samplingPeriod = (filterlist->cutOffAt100Hz*2000)/(sensor->cutOffFreq*100); 
	sensor->samplingfreq = samplingPeriod & 0xFF; 

}