/*
 * payloadProtocol.h
 *
 * Created: 26/10/2018 12:18:48
 *  Author: mads-
 */ 


#ifndef PAYLOADPROTOCOL_H_
#define PAYLOADPROTOCOL_H_
#include "Initialize_node.h"


extern void decodeHubServiceMessage(uint8_t message_array[8], sensor_at_node*);

extern void sendServiceMessage(uint8_t type, uint8_t unit, uint8_t range_min, uint8_t range_max, uint8_t trans_frq, uint8_t sampl_frq, uint8_t filt_type, uint8_t filt_par);

extern void setFilterParameters(uint8_t type, float aCoef, float bCoef, Filter *parameters);

#endif /* PAYLOADPROTOCOL_H_ */
