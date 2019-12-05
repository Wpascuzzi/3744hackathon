/*
 * usart.c
 *
 * Created: 4/6/2018 9:21:33 PM
 * Author: Wes Piard
 * Modified By: Chris Crary & Will Pascuzzi
 * Last Modified: 2 July, 2019
 */

#include "usart.h"

/*****************************************************************************
* Name: USARTF0_init
* Purpose: To configure the USARTF0 module for a specific asynchronous
*          serial protocol, and to enable the corresponding receiver and
*          transmitter.
* Input(s): N/A
* Output: N/A
******************************************************************************/
void USARTF0_INIT(void)
{
  /* configure relevant TxD and RxD pins */

  /* configure baud rate */
	/* At 2 MHz SYSclk, 0 BSEL, 0 BSCALE corresponds to 125000 bps */
	USARTF0.BAUDCTRLA = (uint8_t)0;
	USARTF0.BAUDCTRLB = (uint8_t)( (0 << 4) | (0 >> 8) );

  /* configure remainder of serial protocol */
  /* in this example, a protocol with 8 data bits, no parity, and
   * one stop bit is chosen. */
	USARTF0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc |
					        USART_PMODE_DISABLED_gc		  |
					        USART_CHSIZE_8BIT_gc        &
                  ~USART_SBMODE_bm;

  /* enable receiver and/or transmitter systems */
	USARTF0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

  /* enable interrupt (optional) */
	/* USARTF0.CTRLA = USART_RXCINTLVL_MED_gc; */
}

/*****************************************************************************
* Name: USARTF0_out_char
* Purpose: To output a character from the transmitter within USARTF0.
* Input(s): c (char)
* Output: N/A
******************************************************************************/
void USARTF0_out_char(char c)
{
	while(!(USARTF0.STATUS & USART_DREIF_bm));
	USARTF0.DATA = c;
}

/*****************************************************************************
* Name: USARTF0_out_string
* Purpose: To output a C string from the transmitter within USARTF0.
* Input(s): str (char *)
* Output: N/A
******************************************************************************/
void USARTF0_out_string(char * str)
{
	while(*str!= 0) USARTF0_out_char(*(str++));
}

/*****************************************************************************
* Name: USARTF0_in_char
* Purpose: To read in a character from the receiver within USARTF0.
* Input(s): N/A
* Output: (char)
******************************************************************************/
char USARTF0_in_char(void)
{
 while(!(USARTF0.STATUS & USART_RXCIF_bm));
 return USARTF0.DATA;
}

/*****************************************************************************
* Name: USARTF0_in_string
* Purpose: To read in a string from the receiver within USARTF0. The string
*          is to be stored within a pre-allocated buffer (buf).
* Input(s): buf (char *)
* Output: N/A
******************************************************************************/
void USARTF0_in_string(char * buf)
{
  /* intentionally left blank */
}