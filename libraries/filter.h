/*
 * filter.h
 *
 * Created: 24/11/2018 13:16:16
 *  Author: mads-
 */ 


#ifndef FILTER_H_
#define FILTER_H_

#include <stdint.h>

typedef struct
{
	float vBuffer[2];
	uint8_t pointer;
}bufferElement;

typedef struct
{
	uint8_t order;
	uint8_t cutOffAt100Hz;
	float aCoefs[5][2];
	float bCoefs[5][3];
} Filter;


extern float calculateFilterAlternative(float input, Filter* filter, bufferElement* buffers);

extern float secondOrderCascadeAlternative(float aCoef[2], float bCoef[3], bufferElement* buffer, float input);

#endif /* FILTER_H_ */