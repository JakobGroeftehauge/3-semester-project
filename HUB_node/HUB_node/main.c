/*
 * Hub_CAN_Ex.c
 *
 * Created: 10/21/2013 4:14:22 PM
 *  Author: Digi-Key Design Support
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "LED.h"
#include "Timer.h"
#include "can_lib.h"
#include <util/delay.h>
#include <stdint.h>


//#define ID_TAG_BASE 0x80
#define DATA_BUFFER_SIZE 2 // Up to 8 bytes Max -- Payload
#define FALSE 0 // Def of booleans
#define TRUE 1 // Def of booleans


#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))


volatile uint8_t CTC_flag; //Volatile --> it can change, for the compiler,
							//unsigned integer 8 bit - also char 

void sys_init(void); //Definition of functions. Functions are described below main. Functions can also be described above main
void io_init(void);


int main(void)
{



	uint8_t i; //Counter used for clearing payload_buffer

	
	sys_init(); // Initialize I/O, Timer, and CAN peripheral
	
	
	sei(); // Enable global interrupts for Timer execution
	
	
	
	uint8_t payload_buffer[DATA_BUFFER_SIZE]; // setup buffer for payload
	st_cmd_t message_object; // Remote Tx MOb (Requests remote data from Sensor Nodes)
	
	message_object.pt_data = &payload_buffer[0]; //Set data pointer to first element of payload_buffer. 
	message_object.status = 0; // clear status


	// Loop for keep sending message: 
	while(1)
	{

	for(i = 0; i < DATA_BUFFER_SIZE; i++) 
	{
	payload_buffer[i] = 0xCC; 
	} //clear payload_buffer

	// Configure transmission message
	message_object.id.std = 0xCCCC; //Define CAN-ID
	message_object.ctrl.ide = FALSE; //Setup standard CAN frame (Define IDE-bit)
	//message_object.ctrl.rtr = FALSE; //This message object do not expect a reply
	message_object.dlc = DATA_BUFFER_SIZE; //Define size of payload
	message_object.cmd = CMD_TX; //Configure MOb mode (command to execute) (page 233). CMD_TX - transmit message. 

	while(can_cmd(&message_object) != CAN_CMD_ACCEPTED); //Execute command specified in MOb. Check if CAN_CMD is accepted.

	while(can_get_status(&message_object) == CAN_STATUS_NOT_COMPLETED);
	{

	};
	 //Wait for message to be sent

	//delay_ms(10000);

	}

}



void sys_init(void) {
	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80; //Clock prescaler register - Clock prescaler set enable
	CLKPR = 0x00; //See datasheet: set prescaler to 1 
	
	io_init();
	
	can_init(0);
}

void io_init(void) {
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	bit_set(PORTD, BIT(1));
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0b00000100; // 0x40; 
	PORTC = 0x00; // 
	

}
