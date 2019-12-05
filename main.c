	/*
 * HackathonProject.c
 *
 * Created: 12/5/2019 11:03:29 AM
 * Author : willp
 */ 

#include <avr/io.h>
#include "GPIO.h"
#include "usart.h"


int main(void)
{
	uint8_t switchData = 0;
	uint8_t ledData = 0;
	uint8_t attacker = 1;
	
	PORTA_INIT();
	PORTC_INIT();
	PORTF_INIT();
	USARTF0_INIT();
    /* Replace with your application code */
    while (1) 
    {
		
		
		//TODO: set up different game states
		//Preparation state, leds mapped directly
		if(attacker)
		{
		switchData = PORTA.IN;
		ledData = switchData;
		PORTC.OUT = ledData;
		USARTF0_out_char(switchData);
		}
		if(!attacker)
		{
		ledData = USARTF0_in_char();
		PORTC.OUT = ledData;
		}
		 
	}
}

