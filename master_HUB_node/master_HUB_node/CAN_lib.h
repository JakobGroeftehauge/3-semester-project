/*
 * CAN_lib.h
 *
 * Created: 10/30/2018 9:25:52 AM
 *  Author: Jakob
 */ 

 #include <stdbool.h>
 #include <stdint.h>
 #include "CAN_drv.h"


#ifndef CAN_LIB_H_
#define CAN_LIB_H_


#define MSG_SIZE 8



typedef enum {
	TX,
	RX,
	ACCEPTED,
	NOT_ACCEPTED, 
	DONE

				} arbritraryCmd;



typedef  struct{
	uint8_t cmd;
	uint16_t id;
	uint16_t mask; 
	uint8_t dlc;
	uint8_t* pt_data;
	uint8_t MObNumber; 
	uint8_t status;  
				} st_cmd_t;


extern uint8_t can_cmd(st_cmd_t*);

extern uint8_t can_init();

extern void extra_delay(uint16_t delaytimes);

extern void transfer_data(st_cmd_t*);

#endif /* CAN_LIB_H_ */