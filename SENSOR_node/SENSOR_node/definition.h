/*
 * definition.h
 *
 * Created: 25-10-2018 08:41:32
 *  Author: Lars-p
 DATASHEET: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7647-Automotive-Microcontrollers-ATmega16M1-32M1-64M1-32C1-64C1_datasheet.pdf#G1179702
 */ 
	
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef DEFINITION_H_
#define DEFINITION_H_
#endif /* DEFINITION_H_ */

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
#define ADC_setup() (ADCSRA= 1<<ADEN | 1<< ADSC |1<<ADATE| 0<<ADIF|1<< ADIE | 1<< ADPS2| 0<< ADPS1  |0<<ADPS0)
	//1<< ADEN = Enable ADC
	//1<< ADSC = Start ADC
	//1<< ADATE = ADC Auto trigger Enable Bit
	//0<< ADIF = ADC Interrupt Flag (Set by hardware as soon as a conversion is complete and the Data register are updated)
	//1<< ADIE =  ADC Interrupt Enable Bit
	//1<< ADPS2 and 0<< ADPS1 and  ADPS0 = Division factor set to 16 (Page 212)
	
	
#define ADC_pin1() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4|0<<MUX3|0<<MUX2|1<<MUX1|0<<MUX0)
	//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<< ADLAR = Left adjusted ADC result
	//0<< MUX3, MUX2 and MUX0 and 1<<MUX1 = ADC2 (Pin setup, Page 211)}
	
#define ADC_input1() (bit_clear(DDRD,BIT(5)))
	//PORTD5 setup for ADC2 input	
	
	
#define ADC_pin2() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR| 0<<MUX4 | 1<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0)	
	//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<< ADLAR = Left adjusted ADC result
	//1<< MUX3 and 0<< MUX2, MUX0 and MUX1 = ADC8 (Pin setup, Page 211)}
	
	
#define ADC_input2() (bit_clear(DDRC,BIT(4)))
	//PORTE setup for ADC8 input 
	
#define ADC_pin3() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4 | 0<<MUX3 | 0<<MUX2 | 1<<MUX1 | 1<<MUX0)
	//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<< ADLAR = Left adjusted ADC result
	//0<< MUX3, MUX2 and 1<< MUX1 and MUX0 = ADC3 (Pin setup, Page 211)}
		
#define ADC_input3() (bit_clear(DDRD,BIT(6)))
	//PORTD4 setup for ADC3 input
	
	
#define ADC_pin4() (ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4 | 0<<MUX3 | 1<<MUX2 | 0<<MUX1 | 1<<MUX0)
	//0<< REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<< ADLAR = Left adjusted ADC result
	//0<< MUX3, MUX1 and 1<< MUX2 and MUX0 = ADC5 (Pin setup, Page 211)}
	
#define ADC_input4() (bit_clear(DDRB,BIT(2)))
	//PORTD4 setup for ADC5  input


//------------------------------------------------------------------------------
// Definitions for Timer0 - NOT SETUPED YET
//------------------------------------------------------------------------------
#define Timer0CTC() (TT)
//------------------------------------------------------------------------------
// Definitions for Timer1 - NOT SETUPED YET
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//Definitions for TransmissionsTIMER (TIMER0)- NOT DEFINED!
//------------------------------------------------------------------------------
#define TransmissionType1() {}

#define TransmissionType2() {}
	
#define TransmissionType3() {}
	
//------------------------------------------------------------------------------
//Definitions for SamplingsTIMER (TIMER1) - NOT DEFINED!
//------------------------------------------------------------------------------
#define SamplingType1() {}

#define SamplingType2() {}

#define SamplingType3() {}

//------------------------------------------------------------------------------
//Definitions for CAN
//------------------------------------------------------------------------------