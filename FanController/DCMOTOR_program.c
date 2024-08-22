/*
 * DCMOTOR_program.c
 *
 *  Created on: Aug 23, 2024
 *      Author: Ahmed
 */



#include "STD_Types.h"
#include "BIT_Calc.h"
#include"GPIO_interface.h"
#include"DCMOTOR_interface.h"
#include "Timer0_interface.h"
#include "Timer0_private.h"
#include "Timer0_config.h"

void DCMOTOR_Init(void){
	DIO_u8SetPortDirection(DIO_u8PORTB,DIO_u8PORT_OUTPUT); //Set Direction of Hbridge inputs

//let Motor in stop state initailly
	DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN0,0);
		DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN1,0);



}
void DCMOTOR_Rotate(enum DCMOTOR_State state,u8 speed){
	if (state==CW){
		DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN0,1);

	}
	else if(state==Stop){
		DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN0,0);
			DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN1,0);
		}
	else if(state==A_CW){
		DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN1,1);
	}
	//Send_DutyCycle to timer 0
	//Timer0_voidSetCompareMatchValue(speed);
	PWM_TIMER0_Start(speed);
}
