/*
 * Sampling_Data.h
 *
 * Created: 14-11-2018 12:55:53
 *  Author: Peter
 */ 

#include <avr/io.h>
#include "ADC_drv.h"
#include "payloadProtocol.h"
#ifndef SAMPLING_DATA_H_
#define SAMPLING_DATA_H_

extern void		sampleData(sensor_at_node* sensor); //Filter function must be added in function
extern void		Wire_off(uint16_t data,sensor_at_node* sensor);

#endif /* SAMPLING_DATA_H_ */