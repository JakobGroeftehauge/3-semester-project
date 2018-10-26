/*
 * definition.h
 *
 * Created: 25-10-2018 08:41:32
 *  Author: lars-p
 */ 

#include <avr/io.h>


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
#define ADC_setup() {(ADCSRA= 1<<ADEN | 1<< ADSC |1<<ADATE| 0<<ADIF|1<< ADIE | 1<< ADPS2| 1<< ADPS1  |0<<ADPS0}
	//1<< ADEN = Enable ADC
	//1<< ADSC = Start ADC
	//1<< ADATE = ADC Auto trigger Enable Bit
	//0<< ADIF = ADC Interrupt Flag (Set by hardware as soon as a conversion is complete and the Data register are updated)
	//1<< ADIE =  ADC Interrupt Enable Bit
	//1<< ADPS1 and ADPS2 and 0<< ADPS0 = Division factor set to 8 (Page 233)
	
#define ADC_pin1()	{ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR| 0<<MUX4 | 0<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0;
					bit_clear{DDRE,BIT(2);}	
	//0<<REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<<ADLAR = Left adjusted ADC result
	// 0<<MUX3, MUX2, MUX0 and MUX1 = ADC0 (Pin setup, Page 232)}
	
	// PORTE setup for ADC0 input 
	
#define ADC_pin2()	{ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4 | 0<<MUX3 | 0<<MUX2 | 0<<MUX1 | 1<<MUX0;
					bit_clear{DDRE,BIT(4);}
	//0<<REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<<ADLAR = Left adjusted ADC result
	// 0<<MUX3, MUX2 and MUX1 and 1<<MUX0 = ADC1 (Pin setup, Page 232)}
	
	// PORTD4 setup for ADC1 input	
	
#define ADC_pin3()	{ADMUX = 0<<REFS1 | 1<<REFS0 | 1<<ADLAR | 0<<MUX4|0<<MUX3|0<<MUX2|1<<MUX1|0<<MUX0;
					bit_clear{DDRD,BIT(5);}
	//0<<REFS1 and 1<<REFS0 = External Vref and AVcc with external capacitor connected on the AREF pin.
	//1<<ADLAR = Left adjusted ADC result
	// 0<<MUX3, MUX2 and MUX0 and 1<<MUX1 = ADC2 (Pin setup, Page 232)}
	
	// PORTD5 setup for ADC2 input
//------------------------------------------------------------------------------
//Definitions for MOSFET
//------------------------------------------------------------------------------
#define MOSFET_Pin() {bit_set(DDRD,BIT(1);
					  bit_set(PORTD,BIT(1)}
	//PIN USED FOR MOSFET IS NOT DEFINED YET

//------------------------------------------------------------------------------
//Definitions for TIMER
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Definitions for CAN
//------------------------------------------------------------------------------