/*
 * Hub_CAN_Ex.c
 *
 * Created: 10/21/2013 4:14:22 PM
 *  Author: Digi-Key Design Support
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "can_lib.h"
#include <util/delay.h>


#define DATA_BUFFER_SIZE 2 // Up to 8 bytes Max
#define FALSE 0
#define TRUE 1


#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))


volatile uint8_t CTC_flag;

void sys_init(void);
void io_init(void);

int main(void)
{
	sys_init();  //Initialize I/O, CAN etc.

	sei();		// Enable interrupts

	uint8_t i; //Variable used to clear data buffer

	uint8_t incoming_msg_buffer[DATA_BUFFER_SIZE]; //Buffer to store payload from the buffer
	st_cmd_t incoming_msg; //message object to store incoming messages

	// Se
	incoming_msg.pt_data = &incoming_msg_buffer;
	incoming_msg.status = 0; 

	while (1)
	{

	for (i=0; i<DATA_BUFFER_SIZE; i++) 
	{
	incoming_msg_buffer[i]= 0x00;
	};   //Clear data buffer

	incoming_msg.id.std = 0x80; // CAN_ID on hte node
	incoming_msg.ctrl.ide = 0; // This message object accepts only standard (2.0A) CAN frames
	incoming_msg.ctrl.rtr = 0; // this message object is not requesting a remote node to transmit data back
	incoming_msg.dlc = DATA_BUFFER_SIZE; // Number of bytes (8 max) of data to expect
	incoming_msg.cmd = CMD_RX; // assign this as a "Receive Standard (2.0A) CAN frame" message object

	while(can_cmd(&incoming_msg) != CAN_CMD_ACCEPTED); //Wait for the MOb to configure

	while(can_get_status(&incoming_msg) != CAN_STATUS_COMPLETED); //Wait for message to arrive 

		bit_flip(PORTD, BIT(7)); //Turn on/off LED on board
		_delay_ms(2000);


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
	bit_set(PORTD, BIT(1));	  //Turn on LED on the node
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0b00000100; // 0x40;
	PORTC = 0x00; //
	

}
