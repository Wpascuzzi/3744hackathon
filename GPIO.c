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

void PORTE_INIT(void)
{
	PORTE.DIRCLR = PIN0_bm | PIN1_bm; //set e1 and e0 to be input
	PORTE.INT0MASK =  PIN0_bm; //maks e1 and e0
	PORTE.INT1MASK =  PIN1_bm;
	PORTE.INTCTRL = 0b1010; //sets to midlvl interrupts
	PORTE.PIN0CTRL = 0b010;
	PORTE.PIN1CTRL = 0b010;
	
}

void PORTF_INIT(void)
{
	PORTF.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm; //sets 3, 4 and 5 of F to output (TX, ANALOGOUT, and POWERDOWN)
	PORTF.OUTSET = PIN5_bm; //sets POWERDOWN to VCC
	PORTF.OUTSET = PIN3_bm; //TX default val high
	PORTF.DIRCLR = PIN2_bm; //sets RX to input
	
}

void PORTD_INIT(void)
{
	PORTD.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm;
	PORTD.OUTCLR = PIN4_bm | PIN5_bm | PIN6_bm;
}