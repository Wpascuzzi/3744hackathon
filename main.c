	/*
 * HackathonProject.c
 *
 * Created: 12/5/2019 11:03:29 AM
 * Author : willp
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "GPIO.h"
#include "usart.h"
#include "TimerCounter.h"

uint8_t switchData = 0;
uint8_t ledData = 0;
uint8_t attacker = 1;
volatile uint8_t selfReady = 0;
volatile uint8_t otherReady = 0;
volatile uint8_t gameState = 0;
volatile uint8_t s1Set = 0;
volatile uint8_t receivedSet = 0;

int main(void)
{
	
	
	PORTA_INIT();
	PORTC_INIT();
	PORTF_INIT();
	PORTE_INIT();
	USARTF0_INIT();

	PMIC.CTRL |= PMIC_MEDLVLEN_bm; //enable mid lvl interrupts
	sei();
	
    while (1) 
    {
		switch(gameState)
		{
		//preparation stage
		case 0:
			switchData = PORTA.IN;
			ledData = switchData;
			PORTC.OUT = ledData;
			
			if(receivedSet)
			{
				if(USARTF0_in_char() == 'r')
				otherReady = 1;
				receivedSet = 0;
			}
			
			//debouncing
			//TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;
			//while(!(TCC0_INTFLAGS & 1)); //waits for ovf flag to be set
			//TCC0_INTFLAGS = 0; //resets ovf flag
			if(s1Set)
			{	
				selfReady = 1;
				gameState = 1;
				s1Set = 0;
			}
		break;
		
		//waiting stage
		case 1:
			PORTD_OUTSET = PIN6_bm; //sets blue pin
			USARTF0_out_char('r'); //sends ready
			
			if(selfReady && otherReady)
			{
				//may want to add a timer delay to prevent issues
				gameState = 2;
			}
		break;
		
		//transmitting stage
		case 2:
		
			PORTD_OUTCLR = PIN6_bm;
			PORTD_OUTSET = PIN4_bm;
			
		break;
		
		}
		
		
		//TODO: set up different game states
		//Preparation state, leds mapped directly
		if(attacker)
		{
		switchData = PORTA.IN;
		ledData = switchData;
		PORTC.OUT = ledData;
		USARTF0_out_char(switchData);
		}
		
		//defender code
		if(!attacker)
		{
		ledData = USARTF0_in_char();
		PORTC.OUT = ledData;
		}
		 
	}
}


//triggered when S1 is pressed
ISR(PORTE_INT0_vect)
{
	
	//s1 set
	s1Set = 1;
	
}

ISR(USARTF0_RXC_vect)
{
	receivedSet = 1;
}