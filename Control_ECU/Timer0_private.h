/*
 * Timer0_private.h
 *
 *  Created on: Aug 13, 2024
 *      Author: Ahmed
 */

#ifndef TIMER0_PRIVATE_H_
#define TIMER0_PRIVATE_H_

#define TCCR0  *((volatile u8*)  0x53)
//TCCR0
#define TCCR0_FOC0  7
#define TCCR0_WGM00 6
#define TCCR0_COM01 5
#define TCCR0_COM00 4  /* Bits of Compare Output Register*/
#define TCCR0_WGM01 3
/*Bits of Prescalar as a timer  or using an external event as counter*/
#define TCCR0_CS02  2
#define TCCR0_CS01  1
#define TCCR0_CS00  0



#define TCNT0  *((volatile u8*)  0x52)

#define OCR0  *((volatile u8*)  0x5C)

#define TIMSK  *((volatile u8*)  0x59)
#define TIMSK_OCIE0 1
#define TIMSK_TOIE0 0



#define TIFR   *((volatile u8*)  0x58)
#define TIFR_OCF0 1
#define TIFR_TOV0 0


#endif /* TIMER0_PRIVATE_H_ */
