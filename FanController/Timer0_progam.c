/*
 * Timer0_progam.c
 *
 *  Created on: Aug 13, 2024
 *      Author: Ahmed
 */


#include "BIT_Calc.h"
#include "STD_Types.h"
#include "Timer0_config.h"
#include "Timer0_private.h"
#include "Timer0_interface.h"




/*
 * This is a configured program.c for a special timer app that makes generates a PWM signal for DC Motor  and here are the details:
 *
 *
 *
 * Description:
* The function responsible for trigger the Timer0 with the PWM Mode.
* Setup the PWM mode with Non-Inverting.
* Setup the prescaler with F_CPU/8.
* Setup the compare value based on the required input duty cycle
* Setup the direction for OC0 as output pin through the GPIO driver.
* The generated PWM signal frequency will be 500Hz to control the DC
Motor speed.
 */



//Global pointer to function holding address of ISR
static void (*Timer0_pvCallBackFunc) (void)=NULL;

void PWM_TIMER0_Start(u8 duty_cycle){

	//Choosing WaveForm Generation MOde to FastPWM
	Set_Bit(TCCR0,TCCR0_WGM01);
	Set_Bit(TCCR0,TCCR0_WGM00);

    //Choosing Non-inverted Mode
	Set_Bit(TCCR0,TCCR0_COM01);
	Clr_Bit(TCCR0,TCCR0_COM00);

   //Enable Compare Match Interrupt Enable
	Set_Bit(TIMSK,TIMSK_OCIE0);

    //Prescalar:(Division by 8)
	Clr_Bit(TCCR0,TCCR0_CS02);
	Set_Bit(TCCR0,TCCR0_CS01);
	Clr_Bit(TCCR0,TCCR0_CS00);

	//TCCR0 |= (1 << TCCR0_CS01) | (1 << TCCR0_CS00);


	// Set the duty cycle in the compare match register
	    OCR0 = (duty_cycle * 255) / 100;  // Convert percentage to value between 0 and 255


}

u8 ReturnOCR0(void){
	//OCR0=duty_cycle;
	return OCR0;

}
void Timer0_voidSetCompareMatchValue(u8 duty_cycle){

	 OCR0 = (duty_cycle * 255) / 100;  // Convert percentage to value between 0 and 255
}


u8 TIMER0_u8SetCallBack(void (*Copy_pvCallBackFunc)(void)){

		u8 Local_u8ErrorState=0;
		if(Copy_pvCallBackFunc!=NULL){
			Timer0_pvCallBackFunc=*Copy_pvCallBackFunc;
		}
		else{
			Local_u8ErrorState=1;
		}
		return Local_u8ErrorState;
	}



void __vector_10(void) __attribute__((signal));
void __vector_10(void){
	Timer0_pvCallBackFunc(); //name of function is already pointer to function
}

