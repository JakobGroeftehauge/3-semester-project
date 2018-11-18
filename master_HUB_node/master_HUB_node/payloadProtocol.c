/*
 * payLoadProtocol.c
 *
 * Created: 26/10/2018 12:18:10
 *  Author: mads-
 */ 
#include "payloadProtocol.h"



// decodeHubServiceMessage(uint8_t message_array[8],sensor_at_node* sensor) takes the array of message bytes
// and fills out the given sensor struct
void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node* sensor) //Tjekker ikke hvilken sensor, beskeden er til?
{
	sensor->sensor_Type = (message_array[1] & 0b11110000)/16; // Shift left nibble to the right with /16
	sensor->unit = message_array[1] & 0b00001111;
	//sensor->range_min = message_array[2];
	//sensor->range_max = message_array[3];
	//sensor->transmission_frequency = (message_array[4] & 0b11110000)/16; // Shift left nibble to the right with /16
	//sensor->sampling_frequency = (message_array[4] & 0b00001111);
	//sensor ->filter_type = message_array[5];
	//sensor ->filter_parameter = message_array[6];
	
	//ACK_FROM_NODE(sensor); //NEEDS TO BE MADE!. TRANSMIT THE STRUCT BACK
}

// sendServiceMessage puts parameters into array, which can be sent
void sendServiceMessage(sensor_at_node* sensorAtNode, st_cmd_t* transmitMOb)//sensor_Types type, units unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par)
{
{	

	for (uint8_t i = 0; i < sensorAtNode->totalNumberOfpolynomials; i++)
	{
	transmitMOb->pt_data[0] = 0b11000101;
	transmitMOb->pt_data[1] = ((i) << 4) | sensorAtNode->totalNumberOfpolynomials;
	transmitMOb->pt_data[5] = sensorAtNode->polynomialList[i].binCoef & 0xFF;
	transmitMOb->pt_data[4] = sensorAtNode->polynomialList[i].binCoef >> 8 & 0xFF;
	transmitMOb->pt_data[3] = sensorAtNode->polynomialList[i].binCoef >> 16 & 0xFF;
	transmitMOb->pt_data[2] = sensorAtNode->polynomialList[i].binCoef >> 24 & 0xFF;
	transmitMOb->id = sensorAtNode->CAN_ID;
	can_cmd(transmitMOb);
	} 

	transmitMOb->pt_data[0] = 0b11000011;
	
	//transmitMOb->pt_data[1] = sensorAtNode->sensor_Type << 4 | sensorAtNode->unit;
	transmitMOb->pt_data[2] = (sensorAtNode->period);
	transmitMOb->pt_data[1] = sensorAtNode->sensor_Type << 4 | sensorAtNode->unit;
	transmitMOb->pt_data[2] = sensorAtNode->period;
	transmitMOb->pt_data[3] = sensorAtNode->cutOffFreq;
	
	for (uint8_t i = 4; i < 8; i++)
	{
	//transmitMOb->pt_data[i] = sensorAtNode->totalNumberOfpolynomials;
	transmitMOb->pt_data[i] = 0x00; 
	} 

	can_cmd(transmitMOb); //Send first message
	can_cmd(transmitMOb); //send last message

	for (uint8_t i = 0; i < sensorAtNode->totalNumberOfpolynomials; i++)
	{
	transmitMOb->pt_data[0] = 0b11000101;
	transmitMOb->pt_data[1] = ((i+1) << 4) & sensorAtNode->totalNumberOfpolynomials;
	transmitMOb->pt_data[2] = sensorAtNode->polynomialList[i].binCoef & 0xFF;
	transmitMOb->pt_data[3] = sensorAtNode->polynomialList[i].binCoef >> 8 & 0xFF;
	transmitMOb->pt_data[4] = sensorAtNode->polynomialList[i].binCoef >> 16 & 0xFF;
	transmitMOb->pt_data[5] = sensorAtNode->polynomialList[i].binCoef >> 24 & 0xFF;
	can_cmd(transmitMOb);
	}


}


//Decoding message from hub and determinds what kind of message type it is.
//void decodeMessage(st_cmd_t* message_struct)
//{
	//uint8_t message_array[8];
	//for(uint8_t i = 0; i<8; i++)
	//{
		//message_array[i] = message_struct -> pt_data[i];
	//}
	//
	//switch ((message_struct->pt_data[0] & 0b11110000))// only looks a first nibble
	//{
		//case 0b11000000: // CAN ID FOR A SERVICE MESSAGE
		//{
			//if (message_struct->id == Sensor1.CAN_ID) //Sensor1 is a struct of sensor_at_node and needs to be init in main as global and with a can ID;
			//{
				//decodeHubServiceMessage(message_array, &Sensor1);
			//}
			//if (message_struct->id == Sensor2.CAN_ID) //Sensor2 is a struct of sensor_at_node and needs to be init in main as global and with a can ID;
			//{
				//decodeHubServiceMessage(message_array, &Sensor2);
			//}
		//}
		//default:
		//{
			////SEND BACK ERROR?
		//}
		//
	//}
//}