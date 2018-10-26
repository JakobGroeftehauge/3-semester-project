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
	sensor->filter_data =  ((uint16_t)message_array[5] << 8) | message_array[6];
	sensor->sampling_frequency = message_array[7];
}