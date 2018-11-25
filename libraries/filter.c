
#include "filter.h"



void flushBuffers(bufferElement* buffers, uint8_t order)
{
	for(uint8_t i = 0; i < (order+1)/2; i++)
	{
		buffers[i].vBuffer[0] = 0;
		buffers[i].vBuffer[1] = 0;
	}
}

float secondOrderCascadeAlternative(float aCoef[2], float bCoef[3], bufferElement* buffer, float input)
{
	
	float v = input - aCoef[0] * buffer->vBuffer[buffer->pointer] - aCoef[1] * buffer->vBuffer[(buffer->pointer+2-1)%2];
	
	float result = bCoef[0] * v + bCoef[1] * buffer->vBuffer[buffer->pointer] + bCoef[2] * buffer->vBuffer[(buffer->pointer+2-1)%2];

	buffer->pointer = (buffer->pointer + 1) % 2;
	
	buffer->vBuffer[buffer->pointer] = v;
	
	return result;

}

// function that calculates the filter, the pointer to the filter is stored in the sensor struct
//	the buffers are also stored in the sensor struct, for these the address will have to be passed (&)
//	as the buffers are, and have to be, local to the sensor.
float calculateFilterAlternative(float input, Filter* filter, bufferElement* buffers)
{
	volatile float intermediateResult = input;
	for(uint8_t i = 0; i < (filter->order+1)/2;i++)
	{
		intermediateResult = secondOrderCascadeAlternative(filter->aCoefs[i], filter->bCoefs[i], &buffers[i], intermediateResult);
	}
}
