/*
 * CFile1.c
 *
 * Created: 02-11-2018 15:15:28
 *  Author: lars-p
 */ 

#include "Timer_drv.h"

void TimerSetup() //Find new name !!
{
	Timer0_CTC1();
	Timer0_CTC2();
	Timer0_interrupt();
	Timer0_1ms();
}

