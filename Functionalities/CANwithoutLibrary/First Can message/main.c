/*
 * First Can message.c
 *
 * Created: 16-10-2018 16:22:26
 * Author : chris
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))
#define F_CPU 8000000UL

void extra_delay(uint8_t delaytimes);
 



int main(void)
{
	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	bit_set(PORTD, BIT(7));
	
	int8_t hello = 1;

//***** CAN initialization *****************************************************

	CANGCON = ( 1 << SWRES );   // Software reset. Has to be done independently from setting the CAN controller in Enable mode.

	CANTCON = 0x00;         // CAN timing prescaler set to 0;

	CANBT1 = 0x02;      	// Set baud rate to 250kb (assuming 8Mhz IOclk)
	CANBT2 = 0x0C;          // "" See page 240 in the datasheet
	CANBT3 = 0x37; 			// ""

	for ( int8_t mob=0; mob<6; mob++ ) {

		CANPAGE = ( mob << 4 );     	// Selects Message Object 0-5

		CANCDMOB = 0x00;       		// Disable mob

		CANSTMOB = 0x00;     		// Clear mob status register;
	}

	CANPAGE = ( 1 << MOBNB0 );		// Select MOB1

	CANIDM1 = 0x00;   	// Clear Mask, let all IDs pass

	CANIDM2 = 0x00; 	// ""

	CANIDM3 = 0x00; 	// ""

	CANIDM4 = 0x00; 	// ""

	CANGCON |= ( 1 << ENASTB );
	    
    while (1) 
    {
		// Transmit
		CANPAGE = 0x00;		// Select MOb0 for transmission
		
		while ( CANEN2 & ( 1 << ENMOB0 ) ); // Wait for MOb 0 to be free
		
		CANSTMOB = 0x00;    	// Clear mob status register
		
		CANIDT4 = 0x00;     	// Set can id to 7
		
		CANIDT3 = 0x00;		// ""

		CANIDT2 = (0b111 << 5);		// ""
		
		CANIDT1 = 0x00;		// ""

		// CANMSG
		
		CANMSG = 0x88;
		
		CANCDMOB = ( 1 << CONMOB0 ) | ( 1 << DLC0 ); 	// Enable transmission, data length=1 (CAN Standard rev 2.0A)
		
		extra_delay(20);
		
		/*if (hello == 1)
		{
			bit_set(PORTD, BIT(1));
			hello = 0;
		}else{
			bit_clear(PORTD,BIT(1));
			hello = 1;
		}*/
		bit_flip(PORTD, BIT(1));
		
		
		
		
    }
}

void extra_delay(uint8_t delaytimes){
	for (uint8_t i = delaytimes; i > 0; i--){
		_delay_ms(20);
	}
}
