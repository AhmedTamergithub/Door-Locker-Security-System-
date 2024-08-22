/*
 * GPIO_program.c
 *
 *  Created on: Jul 15, 2024
 *      Author: Ahmed
 */

/****************************************************************/
/****************************************************************/
/*****************   Author: Ahmed Tamer   **********************/
/*****************    Layer: MCAL          **********************/
/*****************    Driver:DIO           **********************/
/*****************   Version 1.0.0         **********************/
/****************************************************************/
/****************************************************************/
#include "BIT_Calc.h"
#include "STD_Types.h"
#include "GPIO_config.h"
#include "GPIO_private.h"
#include "GPIO_interface.h"

u8  DIO_u8SetPinDirection (u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Direction){
	u8 Local_u8ErrorState=0;
	if (Copy_u8Pin<=DIO_u8PIN7 )
	{
		if ( Copy_u8Direction==DIO_u8PIN_INPUT){
			switch (Copy_u8Port)
			{
			case DIO_u8PORTA: Clr_Bit(DDRA,Copy_u8Pin);break;
			case DIO_u8PORTB:Clr_Bit(DDRB,Copy_u8Pin); break;
			case DIO_u8PORTC:Clr_Bit(DDRC,Copy_u8Pin); break;
			case DIO_u8PORTD:Clr_Bit(DDRD,Copy_u8Pin); break;
			default:Local_u8ErrorState=1;
			}
		}
			else if ( Copy_u8Direction==DIO_u8PIN_OUTPUT){
				switch (Copy_u8Port)
				{
				case DIO_u8PORTA:Set_Bit(DDRA,Copy_u8Pin);break;
				case DIO_u8PORTB:Set_Bit(DDRB,Copy_u8Pin); break;
				case DIO_u8PORTC:Set_Bit(DDRC,Copy_u8Pin); break;
				case DIO_u8PORTD:Set_Bit(DDRD,Copy_u8Pin); break;
				default:Local_u8ErrorState=1;
				}

			}
			else{ Local_u8ErrorState=1;}


	}
	else {Local_u8ErrorState=1;}

	return Local_u8ErrorState;
}


u8  DIO_u8SetPortDirection (u8 Copy_u8Port, u8 Copy_u8Direction){
	u8 Local_u8ErrorState=0;


	  switch (Copy_u8Port){
	  case DIO_u8PORTA: DDRA=Copy_u8Direction;break;
	  case DIO_u8PORTB: DDRB=Copy_u8Direction;break;
	  case DIO_u8PORTC: DDRC=Copy_u8Direction;break;
	  case DIO_u8PORTD: DDRD=Copy_u8Direction;break;

	  }




	return Local_u8ErrorState;
}




u8  DIO_u8SetPinValue (u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value){
	u8 Local_u8ErrorState=0;
	if ( Copy_u8Pin<=DIO_u8PIN7 )
	{
		if ( Copy_u8Value==DIO_u8PIN_LOW){
			switch (Copy_u8Port)
			{
			case DIO_u8PORTA: Clr_Bit(PORTA,Copy_u8Pin);break;
			case DIO_u8PORTB:Clr_Bit(PORTB,Copy_u8Pin); break;
			case DIO_u8PORTC:Clr_Bit(PORTC,Copy_u8Pin); break;
			case DIO_u8PORTD:Clr_Bit(PORTD,Copy_u8Pin); break;
			default:Local_u8ErrorState=1;
			}
		}
		else if ( Copy_u8Value==DIO_u8PIN_HIGH){
			switch (Copy_u8Port)
			{
			case DIO_u8PORTA:Set_Bit(PORTA,Copy_u8Pin);break;
			case DIO_u8PORTB:Set_Bit(PORTB,Copy_u8Pin); break;
			case DIO_u8PORTC:Set_Bit(PORTC,Copy_u8Pin); break;
			case DIO_u8PORTD:Set_Bit(PORTD,Copy_u8Pin); break;
			default:Local_u8ErrorState=1;
			}

		}
		else{ Local_u8ErrorState=1;}


	}

	else {Local_u8ErrorState=1;}

	return Local_u8ErrorState;
}







u8  DIO_u8SetPortValue (u8 Copy_u8Port, u8 Copy_u8Value){
	u8 Local_u8ErrorState=0;


	switch (Copy_u8Port){
		  case DIO_u8PORTA: PORTA=Copy_u8Value;break;
		  case DIO_u8PORTB: PORTB=Copy_u8Value;break;
		  case DIO_u8PORTC: PORTC=Copy_u8Value;break;
		  case DIO_u8PORTD: PORTD=Copy_u8Value;break;

		  }

	return Local_u8ErrorState;
}

u8  DIO_u8GetPinValue (u8 Copy_u8Port, u8 Copy_u8Pin, u8* Copy_pu8Value){

	u8 Local_u8ErrorState=0;
	if((Copy_pu8Value!=NULL) && (Copy_u8Pin<=DIO_u8PIN7)){
		switch (Copy_u8Port){
				  case DIO_u8PORTA: * Copy_pu8Value =Copy_u8Pin ;break;
				  case DIO_u8PORTB: * Copy_pu8Value =Copy_u8Pin ;break;
				  case DIO_u8PORTC: * Copy_pu8Value =Copy_u8Pin ;break;
				  case DIO_u8PORTD: * Copy_pu8Value =Copy_u8Pin ;break;
				  default: Local_u8ErrorState=1;
				  }
	}
	else{Local_u8ErrorState=1;}

	return Local_u8ErrorState;
}


