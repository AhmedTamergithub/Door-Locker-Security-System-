/*
 * TWI_program.c
 *
 *  Created on: Aug 6, 2024
 *      Author: Ahmed
 */

#include "BIT_Calc.h"
#include "STD_Types.h"
#include"TWI_interface.h"
#include"TWI_config.h"
#include"TWI_private.h"


void I2C_voidInit(const TWI_ConfigType *Config_Ptr){


	//Enable TWI
		Set_Bit(TWCR,TWCR_TWEN);


		//Set TWBR TO 2

		TWBR=(Config_Ptr->bit_rate);
		//Set Clock Frequency to 400kbps
		Clr_Bit(TWSR,TWSR_TWPS1);
		Clr_Bit(TWSR,TWSR_TWPS0);
		/* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
		       General Call Recognition: Off */

		   //TWAR = 0b00000010;

			TWAR=Config_Ptr->address;

}
void I2C_voidStart(void){

	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWSTA)|(1<<TWCR_TWEN);

	 while (!(TWCR & (1<<TWCR_TWINT)));
	// if ((TWSR & 0xF8) != START)
	 //ERROR();

}
void I2C_voidStop(void){

	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN)|(1<<TWCR_TWSTO);


}
/*This function sends slave address(7 bits+R/W Bit) or data bits in TWDR register*/
void I2C_voidWrite(u8 Copy_u8Data){
	TWDR =  Copy_u8Data;
	 TWCR = (1<<TWCR_TWINT) | (1<<TWCR_TWEN);

	 while (!(TWCR & (1<<TWCR_TWINT)));
}

u8 I2C_u8Read(void){
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN);
		/* Wait for TWINT flag set in TWCR Register (data received successfully) */
		while(Get_Bit(TWCR,TWCR_TWINT)!=1);
		/* Read Data */
		return TWDR;


}
void I2C_voidSendACK(void){
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1<<TWCR_TWEA);
	while(Get_Bit(TWCR,TWCR_TWINT)!=1);
}
void I2C_voidSendNACK(void){
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) ;
		while(Get_Bit(TWCR,TWCR_TWINT)!=1);
}




u8 I2C_u8GetStatus(void){
	//This function returns TWSR register that has the operations status of TWI
	u8 I2C_u8Status = TWSR & 0xF8;//Masking the first most significant 5 bits

		return I2C_u8Status;

}




















































/*
void TWI_voidMasterInit(void){

	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	//Set TWBR TO 2
	TWBR=2;
	//Set Clock Frequency to 400kbps
	Clr_Bit(TWSR,TWSR_TWPS1);
	Clr_Bit(TWSR,TWSR_TWPS0);
	// Enable Acknowledge Bit
	//Set_Bit(TWCR,TWCR_TWEA);
	//initialize address

	if(Copy_u8SlaveAddress!=0){
		TWAR=Copy_u8SlaveAddress<<1;
	}

}

void TWI_voidSlaveInit(u8 Copy_u8SlaveAddress){
	//initialize address
	if(Copy_u8SlaveAddress!=0){
		TWAR=Copy_u8SlaveAddress<<1;
	}


	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	// Enable Acknowledge Bit
	//Set_Bit(TWCR,TWCR_TWEA);
}


void TWI_SendStartCondition(void){

	TWI_ErrorStatus Local_ErrStatus=NoError;

	//Enable Start Condition
	Set_Bit(TWCR,TWCR_TWSTA);

	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);

	//TWI Check that HW raised the TWINT FLAG indicating start condition is sent
	while(Get_Bit(TWCR, TWCR_TWINT )!=1);

	//Checking status of bus after sending start condition
	if ((TWSR & 0xF8) != START_ACK){
		return  Start_Error ;
	}
	else {return NoError;}
	//Ask in main.c if return equal to Send Start Condition then call send slave address with write(VERY IMPORTANT)
	//and according to R/W bit Call Slave_address_read or slave_address_write function
}





void TWI_SendRepeatedStart(void){
	TWI_ErrorStatus Local_ErrStatus=NoError;

	//Enable Start Condition
	Set_Bit(TWCR,TWCR_TWSTA);

	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);

	//TWI Check that HW raised the TWINT FLAG indicating start condition is sent
	while(Get_Bit(TWCR, TWCR_TWINT )!=1);

	//Checking status of bus after sending start condition
	if ((TWSR & 0xF8) != REP_START_ACK){
		return  Start_Error ;
	}
	else {return NoError;}
}




void TWI_SendRSlaveAddressWithWrite(u8 Copy_u8SlaveAddress){
	//TWI_ErrorStatus Local_ErrStatus=NoError;
	//clear TWSTA Flag by writing zero in it
	Clr_Bit(TWCR,TWCR_TWSTA);
	//Set Slave Address in 7 MOST SIGNIFICANT BITS Data Register
	TWDR=Copy_u8SlaveAddress<<1;
	Clr_Bit(TWDR,0);//ENSURE WRITE SIGNAL
	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);

	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);


	//TWI Check that HW raised the TWINT FLAG indicating slave address is written with ack
	while(Get_Bit(TWCR, TWCR_TWINT )!=1);

	if ((TWSR & 0xF8) != SLAVE_ADD_AND_WR_ACK)
	{return SlaveAddressandWriteAck_Error;}
	else
	{return NoError;}
	//If return value=0 go to function of master write databyte


}

void TWI_SendRSlaveAddressWithRead(u8 Copy_u8SlaveAddress){
	//TWI_ErrorStatus Local_ErrStatus=NoError;
	//clear TWSTA Flag by writing zero in it
	Clr_Bit(TWCR,TWCR_TWSTA);
	//Set Slave Address in 7 MOST SIGNIFICANT BITS Data Register
	TWDR=Copy_u8SlaveAddress<<1;
	Set_Bit(TWDR,0);//ENSURE READ SIGNAL
	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);
	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	//TWI Check that HW raised the TWINT FLAG indicating slave address is read with ack
	while(Get_Bit(TWCR, TWCR_TWINT )!=1);



	if ((TWSR & 0xF8) != SLAVE_ADD_AND_RD_ACK)
	{return SlaveAddressandReadAck_Error;}

	else {return NoError;}
	//If return value=0 go to function of master read databyte

}

void TWI_MasterWriteDataByte(u8 Copy_u8DataByte){

	//TWI_ErrorStatus Local_ErrStatus=NoError;
	//Write in TWDR
	TWDR=Copy_u8DataByte;

	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);

	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);
	while (Get_Bit(TWCR, TWCR_TWINT )!=1);
	if ((TWSR & 0xF8) != MSTR_WR_BYTE_ACK){
		return MasterWriteDataByte_Error;
	}
	else {return NoError;}
}

u8 TWI_MasterReadDataByte(u8 *Copy_u8DataByte){
	TWI_ErrorStatus Local_ErrStatus=NoError;
	//reading the content
	*Copy_u8DataByte=TWDR;

	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);
	//Enable TWI
	Set_Bit(TWCR,TWCR_TWEN);

	//Enable TWI with ACK
	Set_Bit(TWCR,TWCR_TWEA);

	while (Get_Bit(TWCR, TWCR_TWINT)!=1);
	if ((TWSR & 0xF8)!=MSTR_RD_BYTE_WITH_ACK){
		return  MasterReadDataByte_Error;

	}
	else {return NoError;}
	// if return value=0,then go to stop condition function
}



void TWI_SendStopCondition(void){

	Set_Bit(TWCR,TWCR_TWEN);
	Set_Bit(TWCR,TWCR_TWSTO);

	//Clear Flag of TWINT by writing 1 in it
	Set_Bit(TWCR,TWCR_TWINT);

}
*/

//u8 TWI_readByteWithNACK(void)
//{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */

	//TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	//while(Get_Bit(TWCR,TWCR_TWINT)!=1);
	/* Read Data */
	//return TWDR;
  //}


