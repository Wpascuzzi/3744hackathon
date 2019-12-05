/*
 * GPIO.h
 *
 * Created: 12/5/2019 11:42:17 AM
 *  Author: willp
 */ 

#include <avr/io.h>
# include <avr/interrupt.h>
#ifndef GPIO_H_
#define GPIO_H_

void PORTA_INIT(void);

void PORTC_INIT(void);

void PORTF_INIT(void);

void PORTE_INIT(void);



#endif /* GPIO_H_ */