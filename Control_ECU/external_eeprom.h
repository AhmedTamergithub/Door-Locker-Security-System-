 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "BIT_Calc.h"
#include "STD_Types.h"



/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

u8 EEPROM_writeByte(u16 Copy_u16addr,u8 Copy_u8data);
u8 EEPROM_readByte(u16 Copy_u16addr,u8 *Copy_u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
