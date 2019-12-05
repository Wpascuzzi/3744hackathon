/*
 * usart.h
 *
 * Created: 4/6/2018 9:21:44 PM
 * Author: Wes Piard
 * Modified By: Chris Crary
 * Last Modified: 2 July, 2019
 */


#include <avr/io.h>

#ifndef USART_H_
#define USART_H_

/*****************************************************************************
* Name: USARTF0_init
* Purpose: To configure the USARTF0 module for a specific asynchronous
*          serial protocol, and to enable the corresponding receiver and
*          transmitter.
* Input(s): N/A
* Output: N/A
******************************************************************************/
void USARTF0_INIT(void);

/*****************************************************************************
* Name: USARTF0_out_char
* Purpose: To output a character from the transmitter within USARTF0.
* Input(s): c (char)
* Output: N/A
******************************************************************************/
void USARTF0_out_char(char c);

/*****************************************************************************
* Name: USARTF0_out_string
* Purpose: To output a C string from the transmitter within USARTF0.
* Input(s): str (char *)
* Output: N/A
******************************************************************************/
void USARTF0_out_string(char * str);

/*****************************************************************************
* Name: USARTF0_in_char
* Purpose: To read in a character from the receiver within USARTF0.
* Input(s): N/A
* Output: (char)
******************************************************************************/
char USARTF0_in_char(void);

/*****************************************************************************
* Name: USARTF0_in_string
* Purpose: To read in a string from the receiver within USARTF0. The string
*          is to be stored within a pre-allocated buffer (buf).
* Input(s): buf (char *)
* Output: N/A
******************************************************************************/
void USARTF0_in_string(char * buf);

#endif /* USART_H_ */