/*
 * LCD_interface.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Ahmed
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_


void LCD_voidSendCommand(u8 command);
void LCD_voidSendData(u8 data);
void LCD_voidInit();
void LCD_voidSendString(u8 String[]);
void LCD_voidSendNumber (s32 num);
void LCD_voidSetPos(u8 x, u8 Y);
void LCD_voidSpecialCharacter (u8 block_no , u8 *special, u8 x, u8 y);
void LCD_voidClear();
#endif /* LCD_INTERFACE_H_ */
