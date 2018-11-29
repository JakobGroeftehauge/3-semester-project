/*
 * ADC_drv.h
 *
 * Created: 13-11-2018 15:24:36
 *  Author: lars-p
 */ 

#include "avr/io.h"
#ifndef ADC_DRV_H_
#define ADC_DRV_H_
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
#define ADC_setup() (ADCSRA= 1<<ADEN | 0<< ADSC |0<<ADATE| 0<<ADIF|0<< ADIE | 1<< ADPS2| 0<< ADPS1  |0<<ADPS0)
//1<< ADEN = Enable ADC
//0<< ADSC = Start ADC
//0<< ADATE = ADC Auto trigger Enable Bit (NOT ENABLED)
//0<< ADIF = ADC Interrupt Flag (Set by hardware as soon as a conversion is complete and the Data register are updated)
//0<< ADIE =  ADC Interrupt NOT Enable Bit
//1<< ADPS2 and 0<< ADPS0 and ADPS1  = Division factor set to 16 (Page 212)

#define ADC_setup2() (ADMUX = 0<<REFS1 | 1<<REFS0 | 0<<ADLAR | 0<<MUX4|0<<MUX3|0<<MUX2|0<<MUX1|0<<MUX0)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//0<< ADLAR = NOT Left adjusted ADC result
//0<< MUX3, MUX2, MUX0 and MUX1 = ADC2 (Pin setup, Page 211)}

#define ADC_Start() (bit_set(ADCSRA,BIT(6)))
//1<< ADSC = Start ADC

#define ADC_input(ch) (ADMUX = (ADMUX & 0xE0)|ch)
//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
//0<< ADLAR = NOT Left adjusted ADC result
//0<< MUX3, MUX2 and MUX0 and 1<<MUX1 = ADC2 (Pin setup, Page 211)}

#define ADC_pin1() (bit_clear(DDRD,BIT(5)))
//PORTD5 setup for ADC2 input

#define ADC_pin2() (bit_clear(DDRB,BIT(6)))
//PORTB6 setup for ADC7  input

#define ADC_pin3() (bit_clear(DDRC,BIT(4)))
//PORTC4 setup for ADC8 input

#define ADC_pin4() (bit_clear(DDRD,BIT(6)))
//PORTD6 setup for ADC3 input




#endif /* ADC_DRV_H_ */