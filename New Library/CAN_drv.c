/*
 * can_drv.c
 	
 	The contents of this file were originally from the CAN Software Library 
 	provided by Atmel. Use of this file is subject to Atmel's End User 
	License Agreement.
 	
 */ 

//******************************************************************************
//! @file $RCSfile: can_drv.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - CAN (Controller Array Network)
//!             - for AT90CAN128/64/32
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "CAN_drv.h"


//_____ D E F I N I T I O N S __________________________________________________

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn can_clear_all_mob
//!
//! This function clears the Mailbox content.
//! It reset CANSTMOB, CANCDMOB, CANIDTx & CANIDMx and clears data FIFO of
//! MOb[0] upto MOb[LAST_MOB_NB].
//!
//! @warning: This version doesn't clears the data FIFO
//!
//! @param none
//!
//! @return none
//------------------------------------------------------------------------------
void can_clear_all_mob(void)
{
uint8_t  mob_number;
/*
    uint8_t  data_index;
*/

    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        CANPAGE = (mob_number << 4);    //Select each message object 0-5 in turn
        
		CANCDMOB = 0x00; // Disable mob
		
		CANSTMOB = 0x00; // Clear mob status register
/*
        for (data_index = 0; data_index < NB_DATA_MAX; data_index++)
        {
            CANMSG = 0;                 //! MOb data FIFO
        }
*/
    }
}

//------------------------------------------------------------------------------
//  @fn can_get_mob_free
//!
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb is available.
//!
//! @warning none.
//!
//! @param  none.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//------------------------------------------------------------------------------
uint8_t can_get_mob_free(void)
{
    uint8_t mob_number, page_saved;

    page_saved = CANPAGE;
    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        Can_set_mob(mob_number);
        if ((CANCDMOB & 0xC0) == 0x00) //! Disable configuration
        {
            CANPAGE = page_saved;
            return (mob_number);
        }
    }
    CANPAGE = page_saved;
    return (NO_MOB);
}

//------------------------------------------------------------------------------
//  @fn can_get_mob_status
//!
//! This function returns information "MOB completed its job"
//! if one of the RXOK or TXOK Flag is set or "MOB not completed its job
//! if no RXOK and TXOK flags are set.
//! Previously, this function checks if the MOb is configured or not and in
//!  case of the MOB not configured, the function returns "MOB_DISABLE".
//!
//! @warning none.
//!
//! @param none.
//!
//! @return MOb Status.
//!          -  MOB_NOT_COMPLETED
//!          -  MOB_TX_COMPLETED
//!          -  MOB_RX_COMPLETED
//!          -  MOB_RX_DLC_WARNING
//!          -  MOB_DISABLE
//!          or should be a combination of the following errors
//!          -  MOB_ACK_ERROR
//!          -  MOB_FORM_ERROR
//!          -  MOB_CRC_ERROR
//!          -  MOB_STUFF_ERROR
//!          -  MOB_BIT_ERROR
//------------------------------------------------------------------------------
uint8_t can_get_mob_status(void)
{
    uint8_t mob_status, canstmob_copy;

    // Test if MOb ENABLE or DISABLE
    if ((CANCDMOB & 0xC0) == 0x00) {return(MOB_DISABLE);}

    canstmob_copy = CANSTMOB; // Copy for test integrity

    // If MOb is ENABLE, test if MOb is COMPLETED
    // - MOb Status = 0x20 then MOB_RX_COMPLETED
    // - MOb Status = 0x40 then MOB_TX_COMPLETED
    // - MOb Status = 0xA0 then MOB_RX_COMPLETED_DLCW
    mob_status = canstmob_copy & ((1<<DLCW)|(1<<TXOK)|(1<<RXOK));
    if ( (mob_status==MOB_RX_COMPLETED) ||   \
         (mob_status==MOB_TX_COMPLETED) ||   \
         (mob_status==MOB_RX_COMPLETED_DLCW) ) { return(mob_status); }

    // If MOb is ENABLE & NOT_COMPLETED, test if MOb is in ERROR
    // - MOb Status bit_0 = MOB_ACK_ERROR
    // - MOb Status bit_1 = MOB_FORM_ERROR
    // - MOb Status bit_2 = MOB_CRC_ERROR
    // - MOb Status bit_3 = MOB_STUFF_ERROR
    // - MOb Status bit_4 = MOB_BIT_ERROR
    mob_status = canstmob_copy & ERR_MOB_MSK;
    if (mob_status != 0) { return(mob_status); }

    // If CANSTMOB = 0 then MOB_NOT_COMPLETED
    return(MOB_NOT_COMPLETED);
}

//------------------------------------------------------------------------------
//  @fn can_get_data
//!
//! This function copy the data from the selected MOb to the address
//! passed as parameter.
//!
//! @warning none.
//!
//! @param CAN message data address.
//!
//! @return none.
//------------------------------------------------------------------------------
void can_get_data(uint8_t* p_can_message_data)
{
    uint8_t data_index;

    for (data_index = 0; data_index < (Can_get_dlc()); data_index++)
    {
        *(p_can_message_data + data_index) = CANMSG;
    }
}

//------------------------------------------------------------------------------
//  @fn can_fixed_baudrate
//!
//! This function programs the CANBTx registers with the predefined values
//! CONF_CANBT1, CONF_CANBT2, CONF_CANBT3.
//!
//! @warning
//!
//! @param (unused!)
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//------------------------------------------------------------------------------
uint8_t can_fixed_baudrate()
{
    Can_reset();
    Can_conf_bt();
    return 1;
}

void setup_receive_mob(st_cmd_t* MObStruct){
	
	CANPAGE = ( MObStruct->MObNumber << MOBNB0);			//Set canpage to the current mob
	
	CANIE2 |= ( 1 << MObStruct->MObNumber);				// Enable interrupts for the current mob
	
	CANIDM1 = (MObStruct->id / 8);   	// Setup mask with the ID's to be received

	CANIDM2 = ((MObStruct->id & 0x07) << 5); 	// ""

	CANIDM3 = 0x00; 	// ""

	CANIDM4 = 0x00; 	// ""
	
	CANCDMOB = ( 1 << CONMOB1) | ( 8 << DLC0);
	
}  