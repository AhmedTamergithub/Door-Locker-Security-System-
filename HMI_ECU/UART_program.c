/*
 * UART_program.c
 *
 *  Created on: Aug 3, 2024
 *      Author: Ahmed
 */

#include "BIT_Calc.h"
#include "STD_Types.h"
#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"

void UART_init(const UART_ConfigType * Config_Ptr){
	u8 Local_u8UCSRCValue=0; //Because UCSRC we don't use Get and Set Bit in this register

	//No interrupt
    Clr_Bit(UCSRB,UCSRB_RXCIE);
	Clr_Bit(UCSRB,UCSRB_TXCIE);
	Clr_Bit(UCSRB,UCSRB_UDRIE);

     // 1 stop bit
	if(Config_Ptr->stop_bit==1){
	Set_Bit(Local_u8UCSRCValue,UCSRC_URSEL); //TO USE UCSRC REGISTER TO WRITE IN IT
	Clr_Bit(Local_u8UCSRCValue,UCSRC_USBS);
	}
	//SET UART Mode to asynchronous
	Clr_Bit(Local_u8UCSRCValue,UCSRC_UMSEL);
    // 8 Data bits
	if(Config_Ptr->bit_data==8){
	Set_Bit(Local_u8UCSRCValue,UCSRC_UCSZ1);
	Set_Bit(Local_u8UCSRCValue,UCSRC_UCSZ0);
	Clr_Bit(UCSRB,UCSRB_UCSZ2);// Adjusting last bit with 0 ,bec databits=8 only
	}
    // No parity
	if(Config_Ptr->parity==0){
	Clr_Bit(Local_u8UCSRCValue,UCSRC_UPM1);
	Clr_Bit(Local_u8UCSRCValue,UCSRC_UPM0);
	}


	UCSRC=Local_u8UCSRCValue;
	//UCSRC=0b11000110;




if(Config_Ptr->baud_rate==9600){
//Set bitrate to 9600 bps (UBRR is set to 51)
	Clr_Bit(UCSRA,UCSRA_U2X);
    UBRRH=0b00000000;//clear 7th bit
    UBRRL=0b00110011; //51

}



    //Enable TX and RX
   	Set_Bit(UCSRB,UCSRB_TXEN);
   	Set_Bit(UCSRB,UCSRB_RXEN);
}

void UART_voidsend(u8 Copy_u8data){
	while(Get_Bit(UCSRA,UCSRA_UDRE)==0);//Polling till the transmit buffer is empty
	UDR=Copy_u8data;
}

u8 UART_voidreceive(void){
	while(Get_Bit(UCSRA,UCSRA_RXC)==0);
	return UDR;

}
