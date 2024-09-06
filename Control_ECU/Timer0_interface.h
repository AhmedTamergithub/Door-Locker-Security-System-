/*
 * Timer0_interface.h
 *
 *  Created on: Aug 13, 2024
 *      Author: Ahmed
 */

#ifndef TIMER0_INTERFACE_H_
#define TIMER0_INTERFACE_H_
void PWM_TIMER0_Start(u8 duty_cycle);

u8 TIMER0_u8SetCallBack(void (*Copy_pvCallBackFunc)(void));
void Timer0_voidSetCompareMatchValue(u8 duty_cycle);
u8 ReturnOCR0(void);

#endif /* TIMER0_INTERFACE_H_ */
