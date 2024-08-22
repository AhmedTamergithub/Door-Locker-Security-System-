/*
 * ADC_program.c
 *
 *  Created on: Aug 10, 2024
 *      Author: Ahmed
 */

#include "BIT_Calc.h"
#include "STD_Types.h"
#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"


void ADC_voidInit(void){
//if( Config_Ptr->ref_volt==5){

	Clr_Bit(ADMUX,ADMUX_REFS1);
	Set_Bit(ADMUX,ADMUX_REFS0);

//}
//Left adjust
	Set_Bit(ADMUX,ADMUX_ADLAR);


//Choosing by default single Conversion Mode


	//Set Prescalar to 8
	//if( Config_Ptr->prescaler==8){
	Clr_Bit(ADCSRA,ADCSRA_ADPS2);
	Set_Bit(ADCSRA,ADCSRA_ADPS1);
	Set_Bit(ADCSRA,ADCSRA_ADPS0);
	//}
	//Enable ADC
	Set_Bit(ADCSRA,ADCSRA_ADEN);


}




u8 ADC_u8GetChannelReading(u8 Copy_u8Channel){

//Masking the least significant 5 bits to choose the channel
	ADMUX&=0b11100000;
	ADMUX|=Copy_u8Channel;


	//Start Conversion
	Set_Bit(ADCSRA,ADCSRA_ADSC);

  //Polling on ADC flag
	while(Get_Bit(ADCSRA,ADCSRA_ADIF)==0);
	Set_Bit(ADCSRA,ADCSRA_ADIF);

	return ADCH;

}
