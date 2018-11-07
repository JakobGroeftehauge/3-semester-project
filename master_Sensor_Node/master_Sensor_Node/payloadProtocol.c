/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"

//Decoding message from hub and determinds what kind of message type it is. 
void decodemessage(st_cmd_t* message_struct)
{
	switch ((message_struct ->pt_data[0]&0b11110000))// only looks a first nibble
	{
		uint8_t message_array[8];
		for(uint8_t i = 0; i>8; i++)
		{
			message_array[i] = message_struct ->pt_data[i];
		}
		case 0b11000000: // CAN ID FOR A SERVICE MESSAGE
		{
			if (message_struct->id == sensor1 -> CAN_ID) //Sensor1 is a struct of sensor_at_node and needs to be init in main as global and with a can ID;
			{
			decodeHubServiceMessage(message_array,sensor1);
			}
			if (message_struct->id == sensor2 -> CAN_ID) //Sensor2 is a struct of sensor_at_node and needs to be init in main as global and with a can ID;
			{
			decodeHubServiceMessage(message_array,sensor2)
			}
		}
		default:
		{
			//SEND BACK ERROR?
		}
		
	}	
}


// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor)
{
	sensor->sensor_Type = (message_array[1] & 0b11110000) >> 4; 
	sensor->unit = message_array[1] & 0b00001111;
	sensor->range_min = message_array[2];
	sensor->range_max = message_array[3];
	sensor->transmission_frequency = message_array[4];
	sensor->transmission_frequency = (message_array[4] & 0b11110000) >> 4; //Needs to be shifted 4, towwards right 
	sensor->filter_data =  ((uint16_t)message_array[5] << 8) | message_array[6];
	sensor->sampling_frequency = message_array[7];
	sensor->sampling_frequency = message_array[4] & 0b00001111;
}

// sendServiceMessage puts parameters into array, which can be sent
void sendServiceMessage(sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
{
	uint8_t messageTX_array[8];
	messageTX_array[1] = (type & 0b11110000) + (unit & 0b00001111);  ////reconsider type (maybe shift the number, so it is easier to set the parameter) 
	messageTX_array[2] = range_min;
	messageTX_array[3] = range_max;
	messageTX_array[4] = (trans_frq & 0b11110000) + (sampl_frq & 0b00001111); //reconsider trans_frq (maybe shift the number, so it is easier to set the parameter) 
	messageTX_array[5] = (filt_type & 0b11110000) + (filt_par & 0b00001111); //reconsider filt_type (maybe shift the number)
}