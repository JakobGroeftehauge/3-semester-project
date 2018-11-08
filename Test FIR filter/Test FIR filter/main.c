/*
 * Test FIR filter.c
 *
 * Created: 11/8/2018 5:26:57 PM
 * Author : Jakob
 */ 

#include <avr/io.h>
#include <util/delay.h>


//#define F_CPU 1600000UL
#define FILTER_LENGTH 10

	volatile uint8_t whereToStoreNext = 0;

	#define bit_set(p,m)		((p) |= (m))
	#define bit_clear(p,m)		((p) &= ~(m))
	#define BIT(x)				(0x01 << (x))

int main(void)
{
 
	bit_set(DDRD, BIT(1));
	uint8_t i; 
	uint8_t buffer[FILTER_LENGTH];
	uint16_t result; 
	uint8_t input = 1; 

    /* Replace with your application code */
    while (1) 
    {

	
	result = 0; 
	buffer[whereToStoreNext] = 0; 
	updatePointer();
	for(i = 0; i < FILTER_LENGTH; i++)
	{
	result += buffer[i]; 
    }

	result = result / FILTER_LENGTH; 

	bit_clear(PORTD, BIT(1));
	_delay_ms(30); 
}

}

void updatePointer(void)
{
	whereToStoreNext++;

	if (whereToStoreNext-1 > FILTER_LENGTH)
	{
	whereToStoreNext = 0; 
	}
}
