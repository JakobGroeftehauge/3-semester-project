/*
 * FilterProcessing.c
 *
 * Created: 06/11/2018 12:34:10
 *  Author: Peter
 */ 

#include "FilterProcessing.h"


uint16_t avgFilter(uint8_t adcHigh, uint8_t adcLow, uint8_t n, uint8_t ordn)
{
	if (start==0)
	{
		dataArray[n];
	}
	int M = n;
	uint16_t sensorData = 0;
	uint16_t filterData = 0;
	uint16_t adcData = adcHigh<<8+ adcLow;
	dataArray[y]=adcData*ordn;
	
	
	for (int i = 0 ; i <10;i++)
	{
		filterData =  filterData+dataArray[i];
	}
	filterData=filterData/M;	
	
	
	if(int Wire_off(filterData)!)
	{
		sensorData = filterData; //variable to be sent	
		return sensorData;	
	}
	else //send warning message
	
	if(y==M)
	{
		y=0;
	}
	if (start==0)
	{
		start++;
	}
	
	y++;
}


int Wire_off(int a )
{
	if (a<102) // 102 = 0,5V
	{
		// FUNCTION TRANSMIT ERROR MESSAGE FOR LOW WIRE-OFF DETECTION!!
		return 1;
	}
	if (a>921) //921 = 4,5 V
	{
		// FUNCTION TRANSMIT ERROR MESSAGE FOR HIGH WIRE-OFF DETECTION!!
		return 1;
	}
	return 0;
}


