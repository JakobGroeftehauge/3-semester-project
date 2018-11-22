/*
 * alertFunction.c
 *
 * Created: 11/21/2018 12:51:32 PM
 *  Author: Jakob
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "alertFunction.h"
//#include <stdio.h>

//void (*foo)(void);
void (*ptr)(void); 
void setupAlertFunction(void *alertPointer)
{
	ptr = &alertPointer;
}

#define bit_flip(p,m)		((p) ^= (m))
#define bit_write(c,p,m)	(c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x)				(0x01 << (x))
#define bit_set(p,m)		((p) |= (m))

void alertFunction()
{

void (*ptr)(void) = &blinkLED;
(*ptr)(); 
}


void blinkLED()
{

for (uint8_t i = 0; i < 250; i++)
{
bit_set(PORTD, BIT(1));
_delay_ms(30);
} 
}