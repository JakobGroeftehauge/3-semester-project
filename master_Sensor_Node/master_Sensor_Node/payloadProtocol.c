/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"


extern void sendFilteretData(sensor_at_node* Sensor)
{
	Sensor->transmissionMOb->pt_data[0] = 0b00110000; // Data message
	Sensor->transmissionMOb->pt_data[1] = (Sensor->sensor_Type<<4)||Sensor->unit;
	uint8_t *vp = (uint8_t *)&Sensor->filterValue;
	Sensor->transmissionMOb->pt_data[2] = vp[3];
	Sensor->transmissionMOb->pt_data[3] = vp[2];
	Sensor->transmissionMOb->pt_data[4] = vp[1];
	Sensor->transmissionMOb->pt_data[5] = vp[0];
	Sensor->transmissionMOb->dlc = 6;
	can_cmd(Sensor->transmissionMOb);
}

// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node* sensor)
{
	sensor->sensor_Type = (message_array[1] & 0b11110000)/16; // Shift left nibble to the right with /16
	sensor->unit = message_array[1] & 0b00001111;
	sensor->range_min = message_array[2];
	sensor->range_max = message_array[3];
	sensor->transmission_frequency = (message_array[4] & 0b11110000)/16; // Shift left nibble to the right with /16
	sensor->sampling_frequency = (message_array[4] & 0b00001111);
	sensor->filter_type = message_array[5];
	sensor->filter_parameter = message_array[6];
	
	//ACK_FROM_NODE(sensor); //NEEDS TO BE MADE!. TRANSMIT THE STRUCT BACK
}

// sendServiceMessage puts parameters into array, which can be sent
void sendServiceMessage(sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
{
	uint8_t messageTX_array[8];
	messageTX_array[1] = (type & 0b00001111) << 4 + (unit & 0b00001111);
	messageTX_array[2] = range_min;
	messageTX_array[3] = range_max;
	messageTX_array[4] = (trans_frq & 0b00001111) << 4 + (sampl_frq & 0b00001111); 
	messageTX_array[5] = (filt_type & 0b00001111) << 4 + (filt_par & 0b00001111); 
}


//Decoding message from hub and determinds what kind of message type it is.
void decodeMessage(st_cmd_t* message_struct,sensor_at_node* SensorList, uint8_t NUMBER_OF_SENSORS)
{
	uint8_t message_array[8];
	for(uint8_t i = 0; i<8; i++)
	{
		message_array[i] = message_struct -> pt_data[i];
	}
	
	switch ((message_struct->pt_data[0] & 0b11110000))// only looks a first nibble
	{
		case 0b11000000: // ID FOR A SERVICE MESSAGE
		{
			for(int i = 0; i < NUMBER_OF_SENSORS;i++)
			{
				if (message_struct->id == SensorList[i].CAN_ID)
				{
					decodeHubServiceMessage(message_array,&SensorList[i]);
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

