/*
 * fan_app.c
 *
 *  Created on: Aug 22, 2024
 *      Author: Ahmed
 */


#include "GPIO_interface.h"
#include "LCD_interface.h"
#include "STD_Types.h"
#include "BIT_Calc.h"
#include "ADC_interface.h"
#include "Timer0_interface.h"
#include "GIE_interface.h"
#include "DCMOTOR_interface.h"
#include <util/delay.h>

enum DCMOTOR_State motor_state;
void algorithm(temperature);


//Global Variables
u8 PWM_value;
	u8 temperature;
	u16 Millivolt;
	u16 reading;
	u8 Fan_state;
  u8 OCR0_state;

void main(void){

	//Initializing LCD port
	DIO_u8SetPortDirection(DIO_u8PORTC, DIO_u8PORT_OUTPUT);
	DIO_u8SetPortDirection(DIO_u8PORTD, DIO_u8PORT_OUTPUT);
	//Set OCO Pin as Output
	DIO_u8SetPinDirection(DIO_u8PORTB,DIO_u8PIN3,DIO_u8PIN_OUTPUT);
	//Initializing ADC Ports
	ADC_ConfigType y= {5,8};
	ADC_ConfigType* ptr=&y;
	ADC_voidInit();

	//LCD initializing
	LCD_voidInit();

	// Clear the LCD screen
	LCD_voidClear();
	GIE_voidEnable();

	while(1){

		   reading=ADC_u8GetChannelReading(2);

		    Millivolt = (reading * 5000UL) /256UL;
			temperature=Millivolt/10;


			algorithm(temperature);


			LCD_voidSetPos(0, 0);
			if(Fan_state==0){
				LCD_voidSendString("FAN OFF");
			}
			if(Fan_state==1){
				//LCD_voidClear();
				LCD_voidSendString("FAN  ON");
						}

			//LCD_voidSendString("Duty Cycle=");
			//OCR0_state=ReturnOCR0();
			//LCD_voidSendNumber(OCR0_state);

			LCD_voidSetPos(1, 0);
			LCD_voidSendString("Temperature :");
			LCD_voidSendNumber(temperature);
			//Another iteration
			//DCMOTOR_Rotate(Stop,0);

	}
}
void algorithm(temperature){

	if(temperature<30){
					Fan_state=0;
					//motor_state=Stop;
					DCMOTOR_Init();
					DCMOTOR_Rotate(Stop,0);
				}
	else if(temperature>=30 && temperature<60){
	Fan_state=1;
	//motor_state=CW;
	//PWM_TIMER0_Start(25);
	DCMOTOR_Init();

  DCMOTOR_Rotate(CW,25);
}

	else if(temperature>=60 && temperature<90 ){
	Fan_state=1;
	//PWM_TIMER0_Start(50);
	//motor_state=CW;
	DCMOTOR_Init();
  DCMOTOR_Rotate(CW,50);
}


	else if(temperature>=90 && temperature<120){
	Fan_state=1;
	//motor_state=CW;
	PWM_TIMER0_Start(75);
	DCMOTOR_Init();
  DCMOTOR_Rotate(CW,75);
}
	else if(temperature>=120){
		Fan_state=1;
		//motor_state=CW;
		//PWM_TIMER0_Start(100);
		DCMOTOR_Init();
	  DCMOTOR_Rotate(CW,100);
	}

}
