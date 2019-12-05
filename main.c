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
volatile uint8_t s0Set = 0;
volatile uint8_t receivedSet = 0;
volatile uint8_t receivedData = 0;
volatile uint8_t enemyData = 0;
volatile uint8_t score = 0;
volatile uint8_t defSet = 0;

int main(void)
{
	
	
	PORTA_INIT();
	PORTC_INIT();
	PORTF_INIT();
	PORTE_INIT();
	USARTF0_INIT();
	PORTD_INIT();
	timer_init(0);

	PMIC.CTRL |= PMIC_MEDLVLEN_bm; //enable mid lvl interrupts
	sei();
	
    while (1) 
    {
		switch(gameState)
		{
		//preparation stage
		case 0:
		selfReady = 0;
		otherReady = 0;
		s1Set = 0;
			PORTD_OUTSET = PIN4_bm | PIN5_bm | PIN6_bm;
			PORTD.OUTCLR = PIN5_bm;
			switchData = PORTA.IN;
			ledData = switchData;
			PORTC.OUT = ledData;
			
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
			//USARTF0_out_char(0xFF);
		break;
		
		//waiting stage
		case 1:
			defSet = 1;
			PORTC.OUT = 0xFF; //clears LEDs
			PORTD_OUTSET = PIN4_bm | PIN5_bm | PIN6_bm;
			PORTD_OUTCLR = PIN6_bm; //sets blue pin
			USARTF0_out_char('r'); //sends ready
			
			
			if(selfReady && otherReady)
			{
				//may want to add a timer delay to prevent issues
				gameState = 2;
			}
			
		break;
		
		//transmitting stage
		case 2:
		PORTC.OUT = 0xFF; //clears LEDs
		PORTD_OUTSET = PIN4_bm | PIN5_bm | PIN6_bm;
		PORTD_OUTCLR = PIN4_bm;
		
		TCC0.CNT = 0;
		TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;
		//attacker transmits first
		if(attacker)
		{
		while(!(TCC0_INTFLAGS & 1))
			{
			
			USARTF0_out_char(ledData); //transmits LED DATA
			
			}
		}
		if(!attacker)
		{
			//for(int j = 20000; j > 0; j--)
			{
				enemyData = USARTF0_in_char();
			}
		}
		
		TCC0_INTFLAGS = 1; //resets ovf flag
			
			gameState = 3;
			
		case 3:
			PORTC.OUT = 0xFF; //clears LEDs
			PORTD_OUTCLR = PIN5_bm;
			TCC0.CNT = 0;
			if(!attacker)
			{
				while(!(TCC0_INTFLAGS & 1))
				{
					
					USARTF0_out_char(ledData); //transmits LED DATA
					
				}
			}
			if(attacker)
			{
				//for(int j = 20000; j > 0; j--)
				{
					enemyData = USARTF0_in_char();
				}
			}
			TCC0_INTFLAGS = 1; //resets ovf flag
			
				gameState = 4;
				
				//compare
		case 4:
			PORTD_OUTCLR = PIN6_bm;
			PORTC.OUT = enemyData;
			
			for(int i = 0; i < 8; i++)
			{
				
				//score if different
				if(attacker)
				{
					if( (enemyData & (1 << i )) != (ledData & (1 << i )) )
						score++;
					
				}
				//score if same
				if(!attacker)
				{
					if( (enemyData & (1 << i )) == (ledData & (1 << i )) )
					score++;
				}
			}
			TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;
			while(!(TCC0_INTFLAGS & 1)); //waits for ovf flag to be set
			TCC0_INTFLAGS = 1; //resets ovf flag
			TCC0.CTRLA = TC_CLKSEL_OFF_gc;
				
			
		gameState = 0;
	
		break;
		
		}
	
		
	}
}


//triggered when S1 is pressed
ISR(PORTE_INT1_vect)
{
	
	//s1 set
	s1Set = 1;
	
}

ISR(PORTE_INT0_vect)
{
	
	//s1 set
	if(gameState == 0 && !defSet)
	{
		attacker = 0;
		defSet = 1;
	}
	if(defSet)
	{
		PORTC_OUT = 0xFF;
		switch((score*8)/37)
		{
			case 8:
			PORTC_OUTCLR = PIN0_bm;
			case 7:
			PORTC_OUTCLR = PIN1_bm;
			case 6:
			PORTC_OUTCLR = PIN2_bm;
			case 5:
			PORTC_OUTCLR = PIN3_bm;
			case 4:
			PORTC_OUTCLR = PIN4_bm;
			case 3:
			PORTC_OUTCLR = PIN5_bm;
			case 2:
			PORTC_OUTCLR = PIN6_bm;
			case 1:
			PORTC_OUTCLR = PIN7_bm;
			break;
			case 0:
			PORTC_OUT = 0xFF;
		}
	}
	
}

ISR(USARTF0_RXC_vect)
{
	
	receivedData = USARTF0_in_char();
	if ((gameState == 0 || gameState == 1) & receivedData == 'r')
	{
		otherReady = 1;
	}

}