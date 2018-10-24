/*
 * IncFile1.h
 *
 * Created: 24-10-2018 10:53:48
 *  Author: lars-p
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#endif /* INCFILE1_H_ */

extern uint8_t findByte(uint8_t);

extern void setSensorType(uint8_t,uint8_t);

extern void setTransmissionFrequency(uint8_t); 

extern void setFilter(uint8_t,uint8_t);

extern void setSamplingFrequency(uint8_t);

extern bool checkParameters();

extern void sendReply();

