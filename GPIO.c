/*
 * GPIO.c
 *
 * Created: 12/5/2019 11:22:11 AM
 *  Author: willp
 */ 

#include "GPIO.h"

void PORTA_INIT(void)
{
	PORTA.DIRCLR = 0xFF; //sets all of PORTA to input
}

void PORTC_INIT(void)
{
	PORTC.DIRSET = 0xFF; //sets all of PORTF to Output
	
}
void PORTF_INIT(void)
{
	PORTF.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm; //sets 3, 4 and 5 of F to output (TX, ANALOGOUT, and POWERDOWN)
	PORTF.OUTSET = PIN5_bm; //sets POWERDOWN to VCC
	PORTF.OUTSET = PIN3_bm; //TX default val high
	PORTF.DIRCLR = PIN2_bm; //sets RX to input
	
}