/*
 * GIE_private.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Ahmed
 */

#ifndef GIE_PRIVATE_H_
#define GIE_PRIVATE_H_
//Register that executes Global Interrupt
#define SREG   *((volatile u8*)  0x5F)
//defining SREG bit for GIE
#define SREG_I 7




#endif /* GIE_PRIVATE_H_ */
