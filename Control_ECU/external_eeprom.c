 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "TWI_interface.h"
#include "TWI_private.h"
#include"TWI_config.h"
#include "BIT_Calc.h"
#include "STD_Types.h"

u8 EEPROM_writeByte(u16 Copy_u16addr, u8 Copy_u8data)
{
	/* Send the Start Bit */
	I2C_voidStart();
    if (I2C_u8GetStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    I2C_voidWrite((u8)(0xA0 | ((Copy_u16addr & 0x0700)>>7)));
    if (I2C_u8GetStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 
		 
    /* Send the required memory location address */
    I2C_voidWrite((u8)(Copy_u16addr));
    if (I2C_u8GetStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* write byte to eeprom */
    I2C_voidWrite(Copy_u8data);
    if (I2C_u8GetStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    I2C_voidStop();
	
    return SUCCESS;
}

u8 EEPROM_readByte(u16 Copy_u16addr, u8 *Copy_u8data)
{
	/* Send the Start Bit */
	I2C_voidStart();
    if (I2C_u8GetStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    I2C_voidWrite((u8)((0xA0) | ((Copy_u16addr & 0x0700)>>7)));
    if (I2C_u8GetStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    I2C_voidWrite((u8)(Copy_u16addr));
    if (I2C_u8GetStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    I2C_voidStart();
    if (I2C_u8GetStatus() != TWI_REP_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    I2C_voidWrite((u8)((0xA0) | ((Copy_u16addr & 0x0700)>>7) | 1));
    if (I2C_u8GetStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *Copy_u8data = I2C_u8Read();
    if (I2C_u8GetStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    I2C_voidStop();

    return SUCCESS;
}
