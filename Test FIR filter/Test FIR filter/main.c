/*
 * Test FIR filter.c
 *
 * Created: 11/8/2018 5:26:57 PM
 * Author : Jakob
 */ 

#include <avr/io.h>
#include <util/delay.h>


//#define F_CPU 1600000UL
#define FILTER_LENGTH 100

	volatile uint8_t whereToStoreNext = 0;

	#define bit_set(p,m)		((p) |= (m))
	#define bit_clear(p,m)		((p) &= ~(m))
	#define BIT(x)				(0x01 << (x))

int main(void)
{
 
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	bit_set(PORTD, BIT(1)); 
	//bit_set(PORTD, BIT(7));
	
	uint8_t i; 
	float buffer[FILTER_LENGTH];
	float result; 
	float input = 180; 
	float coefficient[FILTER_LENGTH] = {1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45,1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45,1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45,1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45,1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45, 1.45, 4.243, 567.3, 2.454, 23.45}; 
	//float multiplier = 0.0045;

    /* Replace with your application code */
    while (1) 
    {

	//bit_set(PORTD, BIT(7));
	result = 0; 
	bit_set(PORTD, BIT(7)); 
	buffer[whereToStoreNext] = input; 
	//bit_clear(PORTD, BIT(1));
	for(i = 0; i < FILTER_LENGTH; i++)
	{
	updatePointer();
	result += buffer[whereToStoreNext] * coefficient[FILTER_LENGTH-(i+1)]; 
    }
	updatePointer(); 

	bit_clear(PORTD, BIT(7));
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
