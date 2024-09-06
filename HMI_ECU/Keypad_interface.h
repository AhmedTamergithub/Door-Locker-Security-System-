/*
 * Keypad_interface.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Ahmed
 */

#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_
#include"Keypad_config.h"
#include "STD_Types.h"
u8 keypad_value1[Row][Column];
u8 keypad_value2[Row][Column];
void KEYPAD_voidInit(void);
u8 KEYPAD_u8GetValue1(void);
char KEYPAD_charGetValue2(void);

#endif /* KEYPAD_INTERFACE_H_ */
