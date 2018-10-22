/*
 * First Can message.c
 *
 * Created: 16-10-2018 16:22:26
 * Author : chris
 */ 

#include <avr/io.h>
#include <stdint.h>

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))


 



int main(void)
{
	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	bit_set(PORTD, BIT(7));

//***** CAN initialization *****************************************************

	CANGCON = ( 1 << SWRES );   // Software reset. Has to be done independently from setting the CAN controller in Enable mode.

	CANTCON = 0x00;         // CAN timing prescaler set to 0;

	CANBT1 = 0x0E;      	// Set baud rate to 125kb (assuming 8Mhz IOclk)
	CANBT2 = 0x04;          // "" See page 240 in the datasheet
	CANBT3 = 0x13; 			// ""

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
    }
}

