/*
 * DCMOTOR_interface.h
 *
 *  Created on: Aug 23, 2024
 *      Author: Ahmed
 */

#ifndef DCMOTOR_INTERFACE_H_
#define DCMOTOR_INTERFACE_H_


enum DCMOTOR_State{
	CW, //clockwise
	A_CW, //anticlockwise
	Stop //stop rotating
};


void DCMOTOR_Init(void);
void DCMOTOR_Rotate( enum DCMOTOR_State state,u8 speed);

#endif /* DCMOTOR_INTERFACE_H_ */
