/*
 * master_Sensor_Node.c
 *
 * Created: 11/6/2018 12:31:02 PM
 * Author : Jakob
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN_lib.h"
#include "payloadProtocol.h"
#include "ADCTimer_drv.h"
#define NUMBER_OF_SENSOR 2
volatile uint8_t tick = 0; // Used by the timer
volatile uint8_t receivedMessages = 0; 
volatile sensor_at_node Sensorlist[NUMBER_OF_SENSOR];
void chip_init (void);
volatile uint8_t data[MSG_SIZE];

int main(void)
{
	
	
//Initialize variable used by main program
	uint8_t samplingCounter1 = 0;
	uint8_t samplingCounter2 = 0;
	uint8_t transmitCounter1 = 0 ;
	uint8_t transmitCounter2 = 0 ;
	


//Setup recieve MOb
	uint8_t recieve_buffer[MSG_SIZE];
	st_cmd_t recieveMOb; 
	recieveMOb.pt_data = &recieve_buffer[0];
	recieveMOb.MObNumber = 0x00; 
	recieveMOb.dlc = MSG_SIZE; 
	recieveMOb.cmd = RX; 
	recieveMOb.mask = 0x0000;
	
//Setup transmit MOb  
	uint8_t transmit_buffer[MSG_SIZE];
	st_cmd_t transmitMOb; 
	transmitMOb.pt_data = &transmit_buffer[0]; 
	transmitMOb.MObNumber = 0x01; 
	transmitMOb.dlc = MSG_SIZE; 
	transmitMOb.cmd = TX; 
	transmitMOb.mask = 0x0001;  

chip_init(); 
can_init(); 
ADCtimerSetup();
can_cmd(&recieveMOb);
Sensorlist[0].CAN_ID = 0x1;
Sensorlist[1].CAN_ID = 0x2;
sei();



while(1)
{
	if (tick>=1)
	{
		tick--;
		samplingCounter1++;
		samplingCounter2++;
		transmitCounter1++;
		transmitCounter2++;

		
		if (receivedMessages > 0)
		{
			transfer_data(&recieveMOb);
			
			decodeMessage(&recieveMOb,&Sensorlist,NUMBER_OF_SENSOR);
			
			transmitMOb.pt_data[0]=Sensorlist[0].CAN_ID;
			transmitMOb.pt_data[1]=recieveMOb.id;
			transmitMOb.pt_data[2]=Sensorlist[0].sampling_frequency;
			
			transmitMOb.pt_data[4]=Sensorlist[1].CAN_ID;
			transmitMOb.pt_data[5]=recieveMOb.id;
			transmitMOb.pt_data[6]=Sensorlist[1].sampling_frequency;
			
			can_cmd(&transmitMOb);
			receivedMessages = 0;
		}
		
		if (samplingCounter1 >= Sensorlist[0].sampling_frequency && Sensorlist[0].sampling_frequency !=0 )
		{
			bit_flip(PORTD, BIT(7));
			samplingCounter1 = 0;
			//Filter(ADC value); //Filter the data
			
		}
		if (samplingCounter2 >= Sensorlist[1].sampling_frequency && Sensorlist[1].sampling_frequency !=0  )
		{
			bit_flip(PORTD, BIT(1));
			samplingCounter2 = 0;
			//Filter(ADC value); //Filter the data
		}
		
		
		/*if (transmitCounter1 >= Sensor1.transmission_frequency)
		{

			clear_buffer(&transmit_buffer);

			transmitMOb.pt_data[0] = recieveMOb.id &0xFF; 
			// Load message into transmitbuffer

			can_cmd(&transmitMOb);
			transmitCounter1=0;
		}*/
		/*
		if (transmitCounter2 >= Sensor2.transmission_frequency)
		{

			clear_buffer(&transmit_buffer);

			transmitMOb.pt_data[0] = recieveMOb.id &0xFF;
			// Load message into transmitbuffer

			can_cmd(&transmitMOb);
			transmitCounter2=0;
		}
		*/
	}
}
}



void chip_init(void){

	//***** Chip initialization
	DDRC = 4; //Set TXCAN as output and RXCAN as input
	
	bit_set(DDRD, BIT(1));
	bit_set(DDRD, BIT(7));
	
	//bit_set(PORTD, BIT(1));
	//bit_set(PORTD, BIT(7));

}
void clear_buffer(uint8_t buffer[MSG_SIZE])
{
uint8_t u; 

for(u = 0; u < MSG_SIZE; u++)
{
buffer[u] = 0x00;
}

} //Maybe make it a makro 

ISR(TIMER0_COMPA_vect)
{
	tick++;
}

ISR( CAN_INT_vect )
{
	receivedMessages++; 
}

