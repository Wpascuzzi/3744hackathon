/*
 * TimerCounter.c
 *
 * Created: 11/25/2019 5:48:28 PM
 *  Author: willp
 */ 

#include "TimerCounter.h"

#define timer TCC0 //timer counter C0
#define timerIntLvl TC_OVFINTLVL_LO_gc //timer overflow interrupt lvl to low
#define prescaler TC_CLKSEL_DIV1024_gc //1024 prescaler


int period = 1*(2000000/1024);


void timer_init(uint8_t interrupts)
{
	
	//sets period equal to
	timer.PER = period;
	
	//resets count
	timer.CNT = 0;
	
	//enables interrupts
	if(interrupts)
		timer.INTCTRLA = timerIntLvl;	
	
	//starts timer at prescaler
	//timer.CTRLA = prescaler;
	
}


