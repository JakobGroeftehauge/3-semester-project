/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"
extern void decodeCoefficient(sensor_at_node* Sensor,uint8_t message_array[8])
{	
	uint8_t coeffNumber =message_array[1]/16;
	
	Sensor->totalNumberOfpolynomials = message_array[1]&&0b00001111;
	
	uint32_t Buffer;
	Buffer = message_array[2] *4096;
	Buffer +=message_array[3] *256;
	Buffer +=message_array[4] * 16;
	Buffer +=message_array[5];
	switch (message_array[1])
	{
		case 0:
		{
			Sensor->polynomialList[0] = Buffer;
			break;
		}
		case 1:
		{
			Sensor->polynomialList[1] = Buffer;
			break;
		}
		case 2:
		{
			Sensor->polynomialList[2] = Buffer;
			break;
		}
		case 3:
		{
			Sensor->polynomialList[3] = Buffer;
			break;
		}
		case 4:
		{
			Sensor->polynomialList[4] = Buffer;
			break;
		}
	}
	Sensor->transmissionMOb->pt_data [0] = Sensor->polynomialList[0];
	Sensor->transmissionMOb->pt_data [1] = Sensor->polynomialList[1];
	Sensor->transmissionMOb->pt_data [2] = Sensor->polynomialList[2];
	Sensor->transmissionMOb->pt_data [3] = Sensor->polynomialList[3];
	Sensor->transmissionMOb->pt_data [4] = Sensor->polynomialList[4];
	can_cmd(Sensor->transmissionMOb);
}

extern void	ACK_TO_Hub(sensor_at_node* Sensor)			// Takes data from the struct and sends it back to the hub. The hub should then be able to checkon it.
{
	Sensor->transmissionMOb->pt_data[0] = 0b11000100;
	Sensor->transmissionMOb->pt_data[1] = (Sensor->sensor_Type)*16+ Sensor->unit;
	Sensor->transmissionMOb->pt_data[2] = Sensor->period;
	Sensor->transmissionMOb->pt_data[3] = Sensor->cutOffFreq;
	Sensor ->transmissionMOb->pt_data[4] = Sensor->cutOffFreq;
	Sensor ->transmissionMOb->pt_data[5] = Sensor->samplingfreq;
	Sensor ->transmissionMOb->pt_data[6] = Sensor->totalNumberOfpolynomials;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	
	can_cmd(Sensor->transmissionMOb);
}

extern void sendError(sensor_at_node* Sensor,uint8_t errorType) // Sending an error message with a specific error type defined elswhere.
{
	Sensor->transmissionMOb->pt_data[0] = 0b10100000;
	Sensor->transmissionMOb->pt_data[1] = errorType;
	Sensor ->transmissionMOb->pt_data[2] = 0;
	Sensor ->transmissionMOb->pt_data[3] = 0;
	Sensor ->transmissionMOb->pt_data[4] = 0;
	Sensor ->transmissionMOb->pt_data[5] = 0;
	Sensor ->transmissionMOb->pt_data[6] = 0;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	can_cmd(Sensor->transmissionMOb);
}

extern float runPolynomial(sensor_at_node* sensor)
{
	float result =sensor->polynomialList[0];
	uint32_t filterValue = (sensor->filterValue/1023)*5; // Transform is from being a value between 0 - 1023 to 0v - 5v
	
	for (int i=0; i<sensor->totalNumberOfpolynomials-1;i++)
	{
		result +=sensor->polynomialList[i+1]*(filterValue)^(i+1);
	}
	
	return result;
	
}
extern void sendFilteretData(sensor_at_node* Sensor)		// Sends the filtered data from the sensors given as parameters. Data comes from the struct and will be updated by another function.
{
	uint32_t polynomialValue = runPolynomial(&Sensor);
	Sensor->transmissionMOb->pt_data[0] = 0b00110000; // Data message
	Sensor->transmissionMOb->pt_data[1] = (Sensor->sensor_Type*16)+Sensor->unit;
	uint8_t *vp = (uint8_t *)&polynomialValue;//Sensor->filterValue;
	Sensor->transmissionMOb->pt_data[2] = vp[3];
	Sensor->transmissionMOb->pt_data[3] = vp[2];
	Sensor->transmissionMOb->pt_data[4] = vp[1];
	Sensor->transmissionMOb->pt_data[5] = vp[0];
	Sensor ->transmissionMOb->pt_data[6] = 0;
	Sensor ->transmissionMOb->pt_data[7] = 0;
	can_cmd(Sensor->transmissionMOb);
}

// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node* sensor)
{
	sensor->sensor_Type = (message_array[1] & 0b11110000)/16; // Shift left nibble to the right with /16
	sensor->unit = message_array[1] & 0b00001111;
	sensor->period = message_array[2];
	sensor->cutOffFreq = message_array[3];
}

// sendServiceMessage puts parameters into array, which can be sent
/*
void sendServiceMessage(sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
{
	uint8_t messageTX_array[8];
	messageTX_array[1] = (type & 0b00001111) << 4 + (unit & 0b00001111);
	messageTX_array[2] = range_min;
	messageTX_array[3] = range_max;
	messageTX_array[4] = (trans_frq & 0b00001111) << 4 + (sampl_frq & 0b00001111); 
	messageTX_array[5] = (filt_type & 0b00001111) << 4 + (filt_par & 0b00001111); 
}*/


//Decoding message from hub and determines what kind of message type it is.
void decodeMessage(st_cmd_t* message_struct,sensor_at_node* SensorList, uint8_t NUMBER_OF_SENSORS) // 
{
	uint8_t message_array[8];
	for(uint8_t i = 0; i<8; i++)
	{
		message_array[i] = message_struct -> pt_data[i];
	}
	
	switch ((message_struct->pt_data[0] & 0b11110000))// only looks a first nibble
	{
		case 0b11000101: //ID for setup of Coefficients
		{
			for(int i = 0; i < NUMBER_OF_SENSORS;i++)
			{
				if (message_struct->id == SensorList[i].CAN_ID)
				{
					decodeCoefficient(&SensorList[i],message_array);
					
				}
				break;
			}
			break;
		}
		case 0b11000000: // ID FOR A SERVICE MESSAGE
		{
			for(int i = 0; i < NUMBER_OF_SENSORS;i++)
			{
				if (message_struct->id == SensorList[i].CAN_ID)
				{
					decodeHubServiceMessage(message_array,&SensorList[i]);
					ACK_TO_Hub(&SensorList[i]);
					break;
				}
			}
			break;
		}
		case 0b11100000: // ID FOR DATA RETRIEVING MESSAGE, FROM SPECIFIC MESSAGE
		{
			for(int i = 0; i < NUMBER_OF_SENSORS;i++)
			{
				if (message_struct->id == SensorList[i].CAN_ID)
				{
					sendFilteretData(&SensorList[i]);
					break;
				}
			}
			break;
		}
		
		default:
		{
			//SEND BACK ERROR?
			break;
		}
		
	}
}

