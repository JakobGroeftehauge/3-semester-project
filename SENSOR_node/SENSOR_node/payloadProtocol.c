/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"

// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor)
{
	sensor->sensor_Type = message_array[1] & 0b11110000;
	sensor->unit = message_array[1] & 0b00001111;
	sensor->range_min = message_array[2];
	sensor->range_max = message_array[3];
	sensor->transmission_frequency = message_array[4];
	sensor->transmission_frequency = message_array[4] & 0b11110000;
	sensor->filter_data =  ((uint16_t)message_array[5] << 8) | message_array[6];
	sensor->sampling_frequency = message_array[7];
	sensor->sampling_frequency = message_array[4] & 0b00001111;
}

// sendServiceMessage puts parameters into array, which can be sent
void sendServiceMessage(sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
{
	uint8_t messageTX_array[8];
	messageTX_array[1] = (type & 0b11110000) + (unit & 0b00001111);
	messageTX_array[2] = range_min;
	messageTX_array[3] = range_max;
	messageTX_array[4] = (trans_frq & 0b11110000) + (sampl_frq & 0b00001111);
	messageTX_array[5] = (filt_type & 0b11110000) + (filt_par & 0b00001111);
}
// change filter parameters - type, output coefficients 'a' and input coefficients 'b'. Filter *parameters is a pointer to the struct with filter parameters
void setFilterParameters(float aCoef, float bCoef, kaskadeData* parameters) //Filter and variables must be changed to correct filter struct
{	
	for (int i=0;i<sizeof(aCoef);i++)
	{
		parameters->aCoefficientPointer = aCoef[i]; 
	}
		
	for (int j=0;j<sizeof(bCoef);j++)
	{
		parameters->bCoefficientPointer = bCoef[j];
	}	
}