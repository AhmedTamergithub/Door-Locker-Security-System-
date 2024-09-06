/*
 * Servo_interface.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Ahmed
 */

#ifndef SERVO_INTERFACE_H_
#define SERVO_INTERFACE_H_

#include "BIT_Calc.h"
#include "STD_Types.h"

void TIMER1_voidInit(void);

u8 TIMER1_u8SetCallBack(void (*Copy_pvCallBackFunc)(void));
void Timer1_voidSetCompareMatchValue(u16 Copy_u16Value);
void Timer1_voidSetICR (u16 Copy_u16ICRValue);
void Timer1_disable(void);
#endif /* SERVO_INTERFACE_H_ */
