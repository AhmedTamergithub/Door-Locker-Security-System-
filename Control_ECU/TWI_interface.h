/*
 * TWI_interface.h
 *
 *  Created on: Aug 6, 2024
 *      Author: Ahmed
 */

#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_
#include "BIT_Calc.h"
#include "STD_Types.h"



//Error Status Enum for differntiating location of errors where in sequence of I2C operation

 typedef enum {
	 NoError,
    Start_Error = 4,          // Explicitly assign value 4
    SlaveAddressandWriteAck_Error,      // Automatically assigned value 5
    SlaveAddressandReadAck_Error,       // Automatically assigned value 6
    MasterWriteDataByte_Error,          // Automatically assigned value 7
    MasterReadDataByte_Error            // Automatically assigned value 8
} TWI_ErrorStatus;




typedef u8  TWI_Address;
typedef u8 TWI_BaudRate;
typedef struct{
     TWI_Address address;
     TWI_BaudRate bit_rate;
}TWI_ConfigType;



/*pass 0 in the slave address argument if master will not become a reciever*/

/*
 *
void TWI_voidMasterInit(void);

void TWI_voidSlaveInit(u8 Copy_u8SlaveAddress);

void TWI_SendStartCondition(void);
void TWI_SendRepeatedStart(void);
void TWI_SendRSlaveAddressWithWrite(u8 Copy_u8SlaveAddress);//Pass 0 if write in Lsb
void TWI_SendRSlaveAddressWithRead(u8 Copy_u8SlaveAddress);//Pass 1 if read in LSB
void TWI_MasterWriteDataByte(u8 Copy_u8DataByte);
u8 TWI_MasterReadDataByte(u8 *Copy_u8DataByte);
u8 TWI_readByteWithNACK(void);// This function recieves data sent on the bus
void TWI_SendStopCondition(void);
*/


void I2C_voidInit(const TWI_ConfigType * Config_Ptr);
void I2C_voidStart(void);
void I2C_voidStop(void);
void I2C_voidWrite(u8 Copy_u8Data);
u8 I2C_u8Read(void);
void I2C_voidSendACK(void);
void I2C_voidSendNACK(void);
u8 I2C_u8GetStatus(void);

#endif /* TWI_INTERFACE_H_ */
