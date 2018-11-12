/*
 * ADCTimer_drv.h
 *
 * Created: 02-11-2018 15:13:30
 *  Author: lars-p
 DATASHEET: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7647-Automotive-Microcontrollers-ATmega16M1-32M1-64M1-32C1-64C1_datasheet.pdf#G1179702
 */ 
#include "avr/io.h"

#ifndef ADCTIMER_DRV_H_
#define ADCTIMER_DRV_H_

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
//Definitions for ADC Pins
//------------------------------------------------------------------------------
#define ADC_setup() (ADCSRA= 1<<ADEN | 1<< ADSC |1<<ADATE| 0<<ADIF|0<< ADIE | 1<< ADPS2| 0<< ADPS1  |0<<ADPS0)
//1<< ADEN = Enable ADC
//1<< ADSC = Start ADC
//1<< ADATE = ADC Auto trigger Enable Bit
//0<< ADIF = ADC Interrupt Flag (Set by hardware as soon as a conversion is complete and the Data register are updated)
//0<< ADIE =  ADC Interrupt NOT Enable Bit
//1<< ADPS2 and 0<< ADPS1 and  ADPS0 = Division factor set to 16 (Page 212)


#define ADC_input1() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4|0<<MUX3|0<<MUX2|1<<MUX1|0<<MUX0)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//1<< ADLAR = Left adjusted ADC result
//0<< MUX3, MUX2 and MUX0 and 1<<MUX1 = ADC2 (Pin setup, Page 211)}

#define ADC_pin1() (bit_clear(DDRD,BIT(5)))
//PORTD5 setup for ADC2 input

#define ADC_input2() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4 | 0<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//1<< ADLAR = Left adjusted ADC result
//0<< MUX3 and 1<< MUX2, MUX1 and MUX0 = ADC7 (Pin setup, Page 211)}

#define ADC_pin2() (bit_clear(DDRB,BIT(6)))
//PORTB6 setup for ADC7  input

#define ADC_input3() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR| 0<<MUX4 | 1<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//1<< ADLAR = Left adjusted ADC result
//1<< MUX3 and 0<< MUX2, MUX0 and MUX1 = ADC8 (Pin setup, Page 211)}

#define ADC_pin3() (bit_clear(DDRC,BIT(4)))
//PORTC4 setup for ADC8 input

#define ADC_input4() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4 | 0<<MUX3 | 0<<MUX2 | 1<<MUX1 | 1<<MUX0)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//1<< ADLAR = Left adjusted ADC result
//0<< MUX3, MUX2 and 1<< MUX1 and MUX0 = ADC3 (Pin setup, Page 211)}

#define ADC_pi4() (bit_clear(DDRD,BIT(6)))
//PORTD6 setup for ADC3 input

//------------------------------------------------------------------------------
// Definitions for Timer0 - (Page 91-96)
//------------------------------------------------------------------------------
#define Timer0_CTC1() (TCCR0A = 0<< COM0A1 |0<< COM0A0 | 0<< COM0B1 | 0<< COM0B0 | 1<< WGM01 | 0<< WGM00)
//0<< COM0A1, COM0A0, COM0B1 and COM0B0 = normal port operation (Used to control pin output)
//1<< WGM01 and 0<< WGM02 and WGM00 = CTC MODE (clear timer on compare)

#define  Timer0_CTC2() (TCCR0B = 0<< FOC0A | 0<< FOC0B |0<< WGM02 |0<< CS02 | 1<< CS01 | 1<< CS00)
// FOC0A and FOC0B only used for non-PWM mode.
// WGM02 not used
// 0<< CS02 and 1<< CS01 and CS00 =  Clok/64 (Prescaler)

#define Timer0_interrupt() (TIMSK0 = 0<< OCIE0B | 1<< OCIE0A | 0<< TOIE0)
// 0<<OCIE0B = Timer/Counter Output Compare Match B Interrupt NOT Enable
// 0<<OCIE0A = Timer/Counter Output Compare Match A Interrupt Enable (USE OCR0A (IF OCR0A=124 -> 1000Hz) –> Output Compare Register A)
// 0<<TOIE0  = Timer/Counter0 Overflow Interrupt NOT Enable

#define  Timer0_1ms() (OCR0A=124)

extern void ADCtimerSetup(); 

#endif /* ADCTIMER_DRV_H_ */