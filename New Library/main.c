/*
 * newLibDev.c
 *
 * Created: 10/30/2018 9:24:25 AM
 * Author : Jakob
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN_lib.h"

void can_tx (void);
void chip_init (void);

//st_cmd_t message_rec;

volatile data[MSG_SIZE];

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

int main(){

	chip_init();	// Chip initialization
	can_init(); 	// CAN initialization
	//setup_receive_mob(1, 0);

	sei();
	uint8_t u; //used for iteration loops



	//Initialization of MOb´s used for storing messages

	uint8_t payload_buffer[MSG_SIZE]; // setup buffer for payload
	uint8_t rec_buffer[MSG_SIZE];
	st_cmd_t message_object; // message object, for storing Mob to send
	st_cmd_t message_rec;
	//message_rec.pt_data = &rec_buffer[0];
	message_object.pt_data = &payload_buffer[0]; //Set data pointer to first element of payload_buffer
						
						message_rec.MObNumber = 0x01;
						message_rec.id = 0x0000; //Define CAN-ID on receiving node
						message_rec.dlc = MSG_SIZE; //Define the size of payload - Defined in CAN_lib.h
						message_rec.cmd = RX; //MOb is used for sending data
	
					can_cmd(&message_rec);


	

	
					for(u = 0; u < MSG_SIZE; u++)	
						{
							payload_buffer[u] = 0xC1;  //Load data buffer storing payload to send
						} 

					//Setup MOb parameters on the message to send
					message_object.MObNumber = 0x00; 
					message_object.id = 0x000; //Define CAN-ID on receiving node
					message_object.dlc = MSG_SIZE; //Define the size of payload - Defined in CAN_lib.h
					message_object.cmd = TX; //MOb is used for sending data

					//while(can_cmd(&message_object) != ACCEPTED);  //Wait for the message to be sent
						can_cmd(&message_object);
						bit_flip(PORTD, BIT(7));
						extra_delay(100);
	
						data[0] = 0xCC;

						while(1)
						{

						if (data[0] == 0xCC)
						{
						bit_flip(PORTD, BIT(7));
						

						for(u = 0; u < MSG_SIZE; u++)
						{
							payload_buffer[u] = data[u];  //Load data buffer storing payload to send
						}

						//Setup MOb parameters on the message to send
						message_object.MObNumber = 0x00;
						message_object.id = 0x000; //Define CAN-ID on receiving node
						message_object.dlc = MSG_SIZE; //Define the size of payload - Defined in CAN_lib.h
						message_object.cmd = TX; //MOb is used for sending data
						can_cmd(&message_object);
						data[0] = 0x00; 
						
						}
						extra_delay(100);
						}
					
		bit_flip(PORTD, BIT(7));

		return 0; 
	}




void chip_init(void){

	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	bit_set(PORTD, BIT(1));
	bit_set(PORTD, BIT(7));

}


void extra_delay(uint16_t delaytimes){
	for (uint16_t i = delaytimes; i > 0; i--){
		_delay_ms(30);
	}
}


