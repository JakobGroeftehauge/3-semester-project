/*
 * FilterProcessing.h
 *
 * Created: 06-11-2018 12:37:17
 *  Author: Peter
 */ 


#ifndef FILTERPROCESSING_H_
#define FILTERPROCESSING_H_


extern uint16_t avgFilter(uint8_t adcHigh,uint8_t adcLow, uint8_t n, uint8_t ordn);

extern int Wire_off(int a);

uint16_t dataArray[];
int y=0;
int start=0;

#endif /* FILTERPROCESSING_H_ */