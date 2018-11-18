/*
 * CAN_lib.c
 *
 * Created: 10/30/2018 9:25:32 AM
 *  Author: Jakob
 */ 
 #include "config.h"
 #include "CAN_lib.h"
 #include "CAN_drv.h"
 #include <avr/interrupt.h>
 #include <avr/io.h>
 
 
 
 void can_tx(st_cmd_t*); 
 

uint8_t can_cmd(st_cmd_t* MObStruct)
{
uint8_t status; 
switch(MObStruct->cmd)
	{
	case TX:
			can_tx(MObStruct); 
			status = 0; 
			//MObStruct->status = DONE; 
			break; 

	case RX:
			setup_receive_mob(MObStruct);
			status = 0; 
			//MObStruct->status = DONE; 
			break; 

	default:
			//MObStruct->status = NOT_ACCEPTED;
			bit_flip(PORTD, BIT(7));
			status = 0x01;
			break; 
	}
		if (status == 0x00)
		{
		return ACCEPTED;
		}	
		else
		{
		 return NOT_ACCEPTED;
		}	
		//return ACCEPTED;
	
	}

void can_tx(st_cmd_t* MObStruct) {

	uint8_t i;   
	if (MObStruct->MObNumber < 6)
	{
	CANPAGE = MObStruct->MObNumber << MOBNB0;		// Select MOb0 for transmission
	}
	else return; 

	
	
	CANSTMOB = 0x00;    	// Clear mob status register

		CANIDT1 = ((MObStruct->id) / 8) ; 	// Set can id to 0
		CANIDT2 = (((MObStruct->id) & 0x07) << 5);
		CANIDT3 = 0x00;		// ""
		CANIDT4 = 0x00;		// ""
	//Set_can_ID(MObStruct->id);

	for (i=0; i < MSG_SIZE; i++)
	{
	CANMSG = MObStruct->pt_data[i]; 
	}
	
	CANCDMOB = ( 1 << CONMOB0 ) | ( (MObStruct->dlc) << DLC0 ); 	// Enable transmission, data length=1 (CAN Standard rev 2.0B(29 bit identifiers))

	while ( ! ( CANSTMOB & ( 1 << TXOK ) ) );	// wait for TXOK flag set
	// todo: have this use interrupts
	CANCDMOB = 0x00;	// Disable Transmission

	CANSTMOB = 0x00;	// Clear TXOK flag

}

void transfer_data(st_cmd_t* recieveMOb)
{

CANPAGE = recieveMOb->MObNumber << 4;

 recieveMOb->id = CANIDT1 + (CANIDT2*8);
 recieveMOb->id = CANIDT1*8 + (CANIDT2/32);
 

if ( CANSTMOB & ( 1 << RXOK) ){  	// Interrupt caused by receive finished

	recieveMOb->dlc = ( CANCDMOB & 0x0F );	// Save number of bytes to be recieved



	for (int8_t i = 0; i < recieveMOb->dlc; i++){

		recieveMOb->pt_data[i] = CANMSG; 		// Get data, INDX auto increments CANMSG
	} 

	CANCDMOB = (( 1 << CONMOB1 ) | ( 8 << DLC0));  //Enable transmission, set data length to 8. 
	
	CANSTMOB = 0x00; //reset MOb status register

	// Note - the DLC field of the CANCDMO register is updated by the received MOb. If the value differs from expected DLC, an error is set

} 
}


uint8_t can_init()
{
	Can_bit_timing(1);
	can_clear_all_mob();                        // c.f. function in "can_drv.c"
	Can_enable();								// c.f. macro in "can_drv.h"
	Can_enable_chosen_ints();                   // macro in "can_drv.h" - change to enable different general interrupts
	return (1);
}

}