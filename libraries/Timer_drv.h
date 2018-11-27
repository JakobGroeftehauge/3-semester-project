/*
 * ADCTimer_drv.h
 *
 * Created: 02-11-2018 15:13:30
 *  Author: lars-p
 DATASHEET: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7647-Automotive-Microcontrollers-ATmega16M1-32M1-64M1-32C1-64C1_datasheet.pdf#G1179702
 */ 
#include <avr/io.h>

#ifndef TIMER_DRV_H_
#define TIMER_DRV_H_

//------------------------------------------------------------------------------
//Usefully macros
//------------------------------------------------------------------------------
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

//------------------------------------------------------------------------------
// Definitions for Timer0 - (Page 91-96)
//------------------------------------------------------------------------------
#define Timer0_CTC1() (TCCR0A = 0<< COM0A1 |0<< COM0A0 | 0<< COM0B1 | 0<< COM0B0 | 1<< WGM01 | 0<< WGM00)
//0<< COM0A1, COM0A0, COM0B1 and COM0B0 = normal port operation (Used to control pin output)
//1<< WGM01 and 0<< WGM02 and WGM00 = CTC MODE (clear timer on compare)

#define  Timer0_CTC2() (TCCR0B = 0<< FOC0A | 0<< FOC0B |0<< WGM02 |0<< CS02 | 1<< CS01 | 1<< CS00)
// FOC0A and FOC0B only used for non-PWM mode.
// WGM02 not used
// 0<< CS02 and 1<< CS01 and CS00 =  Clock/64 (Prescaler)

#define Timer0_interrupt() (TIMSK0 = 0<< OCIE0B | 1<< OCIE0A | 0<< TOIE0)
// 0<<OCIE0B = Timer/Counter Output Compare Match B Interrupt NOT Enable
// 0<<OCIE0A = Timer/Counter Output Compare Match A Interrupt Enable (USE OCR0A (IF OCR0A=124 -> 1000Hz) –> Output Compare Register A)
// 0<<TOIE0  = Timer/Counter0 Overflow Interrupt NOT Enable

#define  Timer0_1ms() (OCR0A=124); //250

extern void TimerSetup(); 

#endif /* TIMER_DRV_H_ */