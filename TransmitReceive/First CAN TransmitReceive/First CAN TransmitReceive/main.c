/* ********************************************************************
simple program to test and see if CAN is able to transmit a 
message over the CANBUS
********************************************************************/
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void can_tx (void);
void chip_init (void);

volatile int8_t data[8];

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))
#define F_CPU 8000000UL

void extra_delay(uint16_t delaytimes);

//***** Reception ISR **********************************
ISR ( CAN_INT_vect ){  				// use interrupts

  	int8_t length, savecanpage;
	
	savecanpage = CANPAGE;			// Save current MOB

	CANPAGE = CANHPMOB & 0xF0;		// Selects MOB with highest priority interrupt

	if ( CANSTMOB & ( 1 << RXOK) ){  	// Interrupt caused by receive finished							

  		length = ( CANCDMOB & 0x0F );	// DLC, number of bytes to be received

  		for ( int8_t i = 0; i < length; i++ ){
			
	    	data[i] = CANMSG; 		// Get data, INDX auto increments CANMSG
  
			//PORTB = data[0];

	  	} // for

		CANCDMOB = (( 1 << CONMOB1 ) /*| ( 1 << IDE )*/ | ( 8 << DLC0));  // Enable Reception 29 bit IDE DLC8
		
		// Note - the DLC field of the CANCDMO register is updated by the received MOb. If the value differs from expected DLC, an error is set

	} // if (CANSTMOB...	
	
	CANSTMOB = 0x00; 		// Reset reason on selected channel

	CANPAGE = savecanpage;		// Restore original MOB

}

//***** chip Initialization **********************************
void chip_init(void){

	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	bit_set(PORTD, BIT(1));

}

//***** CAN ialization *****************************************************
void can_init(void){     

	CANGCON = ( 1 << SWRES );   // Software reset

	CANTCON = 0x00;         // CAN timing prescaler set to 0;					

	CANBT1 = 0x02;      	// Set baud rate to kb (assuming 8Mhz IOclk)
	                                              
	CANBT2 = 0x04;          // ""

	CANBT3 = 0x13; 		// ""

	for ( int8_t mob=0; mob<6; mob++ ) {  

		CANPAGE = ( mob << 4 );     	// Selects Message Object 0-5

		CANCDMOB = 0x00;       		// Disable mob

		CANSTMOB = 0x00;     		// Clear mob status register;
	}

	CANPAGE = ( 1 << MOBNB0 );		// Selecto MOB1

	CANIE2 = ( 1 << IEMOB1 );   	// Enable interrupts on mob1 for reception and transmission

	CANGIE = ( 1 << ENIT ) | ( 1 << ENRX ) /*| ( 1 << ENTX )*/;   // Enable interrupts on receive
	
	CANIDM1 = 0x00;   	// Clear Mask, let all IDs pass    

	CANIDM2 = 0x00; 	// ""

	CANIDM3 = 0x00; 	// ""

	CANIDM4 = 0x00; 	// ""    	

	CANCDMOB = ( 1 << CONMOB1) | ( 8 << DLC0);  // Enable Reception 29 bit IDE DLC8
	
	CANGCON |= ( 1 << ENASTB );			// Enable mode. CAN channel enters in enable mode once 11 recessive bits have been read

	sei();	

} 

//***** transmit *****************************************************
void can_tx(void) { 

	CANPAGE = 0x00;		// Select MOb0 for transmission
	
	//while ( CANEN2 & ( 1 << ENMOB0 ) ); // Wait for MOb 0 to be free
	
	CANSTMOB = 0x00;    	// Clear mob status register
	
	CANIDT4 = 0x00;     	// Set can id to 0	
	CANIDT3 = 0x00;		// ""

	CANIDT2 = (0b11 << 5);		// ""
	
	CANIDT1 = 0x00;		// ""
	
	//for ( int8_t i = 0; i < 8; ++i ){

	CANMSG = 0x46;  // set message data for all 8 bytes to 55 (alternating 1s and 0s
	CANMSG = 0x55;
	CANMSG = 0x43;
	CANMSG = 0x4B;
	CANMSG = 0x20;
	CANMSG = 0x59;
	CANMSG = 0x4F;
	CANMSG = 0x55;
	
	//} // for
	
	CANCDMOB = ( 1 << CONMOB0 ) | ( 8 << DLC0 ); 	// Enable transmission, data length=1 (CAN Standard rev 2.0B(29 bit identifiers))			

	while ( ! ( CANSTMOB & ( 1 << TXOK ) ) );	// wait for TXOK flag set
// todo: have this use interrupts
	CANCDMOB = 0x00;	// Disable Transmission

	CANSTMOB = 0x00;	// Clear TXOK flag

} 

//***** main *****************************************************
int main(){ 

    chip_init();	// Chip initialization

 	can_init(); 	// Can initialization

	

 	while ( 1 ) {	// Forever

		if (data[0] == 0x46){	// if 1st byta of received data is 0x01
			
			extra_delay(600);
		
			can_tx();	// transmit

			data[0] = 0x00;
			
			bit_flip(PORTD, BIT(7));

		}		

	}

	return(0);

}

void extra_delay(uint16_t delaytimes){
	for (uint16_t i = delaytimes; i > 0; i--){
		_delay_ms(30);
	}
}

