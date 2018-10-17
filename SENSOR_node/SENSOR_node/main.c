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


#define ID_TAG_BASE 0x80
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


}



void sys_init(void) {
	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	io_init();
	can_init(0);
}

void io_init(void) {
	
	// Init PORTB[7:0] // LED port
	DDRB = 0xFF;
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0b00000100; // 0x40;
	PORTC = 0x00; // activate pull-up on PC[7] (button)
	

}