
#include "BIT_Calc.h"
#include "STD_Types.h"
#include "Timer1_config.h"
#include "Timer1_private.h"
#include "Timer1_interface.h"




/*
 * This is a configured program.c for a special timer app that makes a stopwatch  and here are the details:
 *
 * Frequency=8 MHZ
 * Prescalar=64
 * Compare match register(OCR1A;that is 16 bits)=31250
 * CTC Mode on ChannelA
 *Required time=1sec
 *Number of compare match events=4
 *
 */



//Global pointer to function holding address of ISR
static void (*Timer1_pvCallBackFunc) (void)=NULL;

void TIMER1_voidInit(void){

	//Choosing Compare Output Mode for ChannelA (as Normal port operation,where OC1A/OC1B disconnected)
	Clr_Bit(TCCR1A,TCCR1A_COM1A1);
	Clr_Bit(TCCR1A,TCCR1A_COM1A0);

    //Choose Waveform Generation Mode (CTC=0100)
	Clr_Bit(TCCR1B,TCCR1B_WGM13);
	Set_Bit(TCCR1B,TCCR1B_WGM12);
	Clr_Bit(TCCR1A,TCCR1A_WGM11);
	Clr_Bit(TCCR1A,TCCR1A_WGM10);


    // Prescalar :(Division by 1024)
	Set_Bit(TCCR1B,TCCR1B_CS12);
	Clr_Bit(TCCR1B,TCCR1B_CS11);
	Set_Bit(TCCR1B,TCCR1B_CS10);
//Set Compare Match Value
	OCR1A = 1172;
	//Output Compare A Match Interrupt Enable
	Set_Bit(TIMSK,TIMSK_OCIE1A);

}

void Timer1_voidSetCompareMatchValue(u16 Copy_u16Value){

		OCR1A=Copy_u16Value;
}

void Timer1_voidSetICR (u16 Copy_u16ICRValue){

	ICR1=Copy_u16ICRValue;
}
u8 TIMER1_u8SetCallBack(void (*Copy_pvCallBackFunc)(void)){

		u8 Local_u8ErrorState=0;
		if(Copy_pvCallBackFunc!=NULL){
			Timer1_pvCallBackFunc=Copy_pvCallBackFunc;
		}
		else{
			Local_u8ErrorState=1;
		}
		return Local_u8ErrorState;
	}

void Timer1_disable(void){
//stop counter by disabling clk source
	Clr_Bit(TCCR1B,TCCR1B_CS12);
		Clr_Bit(TCCR1B,TCCR1B_CS11);
		Clr_Bit(TCCR1B,TCCR1B_CS10);


}

void __vector_7(void) __attribute__((signal));
void __vector_7(void){
	Timer1_pvCallBackFunc(); //name of function is already pointer to function
}

