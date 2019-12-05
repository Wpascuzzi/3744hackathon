	/*
 * HackathonProject.c
 *
 * Created: 12/5/2019 11:03:29 AM
 * Author : willp
 */ 
/*
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
volatile uint16_t score = 0;
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
	timer_init2(0);
	PORTD_REMAP = PIN0_bm | PIN1_bm |PIN2_bm;

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
		while(!(TCC0_INTFLAGS & 1)); //waits for ovf flag to be set
		TCC0_INTFLAGS = 1; //resets ovf flag
		TCC0.CTRLA = TC_CLKSEL_OFF_gc;
		
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
			
			
			//WIN
				if (score >= 37)
				{
					TCD0.CTRLA = TC_CLKSEL_DIV1024_gc;
					int num = 0;
					while(1)
					{
						num++;
						TCD0.CCA = num;
						TCD0.CCB = num >>8;
						TCD0.CCC = num >>16;
					}
				}
			
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


void timer_init2(uint8_t interrupts)
{
	
	//sets period equal to
	TCD0.PER = 5000;//period3k;
	
	
	//resets count
	TCD0.CNT = 0;
	
	//enables interrupts
	//if(interrupts)
	//TCD0.INTCTRLA = timerIntLvl;
	
	
	TCD0_CTRLB |=  TC0_CCAEN_bm | TC0_CCCEN_bm | TC0_CCBEN_bm | 0b0000011; //enables cc chanels ABC and singleslop wavegn mode
	//starts timer at prescaler
	
	
}
*/







///ANNA CODE VVVVVVVVVVV


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
#include "noise.h"

uint8_t switchData = 0;
uint8_t ledData = 0;
uint8_t attacker = 0;
volatile uint8_t selfReady = 0;
volatile uint8_t otherReady = 0;
volatile uint8_t gameState = 0;
volatile uint8_t s1Set = 0;
volatile uint8_t receivedSet = 0;
volatile uint8_t receivedData = 0;

ISR(TCC1_OVF_vect){
	TCC1.CTRLA = 0x00;
	PORTF.OUTCLR = PIN5_bm; //sets POWERDOWN to VCC, 
	//PORTC.OUT =0x00;
}

int main(void)
{
	PORTA_INIT();
	PORTC_INIT();
	PORTF_INIT();
	PORTE_INIT();
	USARTF0_INIT();
	PORTD_INIT();
	
	//clock_init();
	timer_initnoise(); //TCC0 
	dac_init();
	TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc; 
	dma_init();
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc; //setting ch0 to care about timer counter overflow interrupt

	PMIC.CTRL |= PMIC_MEDLVLEN_bm; //enable mid lvl interrupts
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	while(1){
TCC0.PER = 80;
			PORTF.OUTSET = PIN5_bm; //sets POWERDOWN to VCC, 
			TCC1.CNT = 0x0000;
			TCC1.PER = 0x1F00; //1E00
			TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
	}
	
    while (1) 
    {
		switch(gameState)
		{
		//preparation stage
		case 0:
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
		//USARTF0_out_char(0xFF); //sends ready
		
		//waiting stage
		case 1:
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
		
			PORTD_OUTSET = PIN4_bm | PIN5_bm | PIN6_bm;
			PORTD_OUTCLR = PIN4_bm;
			
			
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

ISR(USARTF0_RXC_vect)
{
	
	receivedData = USARTF0_in_char();
	if ((gameState == 0 || gameState == 1) & receivedData == 'r')
	{
		otherReady = 1;
	}
}