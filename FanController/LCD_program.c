/*
 * LCD_program.c
 *
 *  Created on: Aug 11, 2024
 *      Author: Ahmed
 */


#include "BIT_Calc.h"
#include "STD_Types.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#include "LCD_private.h"
#include "GPIO_Interface.h"
#include <util/delay.h>

void LCD_voidSendCommand(u8 command)
{
#if(LcdMode==8)
	//set the RS value as Low (command =>RS =  0 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RsPin,DIO_u8PIN_LOW);
	//set the RW value as Low (Write  => RW = 0 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RwPin, DIO_u8PIN_LOW);
	//set the E Value as High (High CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin, DIO_u8PIN_HIGH);
	//Send Command
		DIO_u8SetPortValue(Lcd_DCGroup,command);
	//Set the E value as Low (Low CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_LOW);
#elif(LcdMode==4)
	//set the RS value as Low (command =>RS =  0 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RsPin,DIO_u8PIN_LOW);
	//set the RW value as Low (Write  => RW = 0 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RwPin,DIO_u8PIN_LOW);
	//set the E Value as High (High CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_LOW);
	//Send Command
		DIO_u8SetPinValue(Lcd_DCGroup,Lcd_D4,Get_Bit(command,4));
		DIO_u8SetPinValue(Lcd_DCGroup,Lcd_D5,Get_Bit(command,5));
		DIO_u8SetPinValue(Lcd_DCGroup,Lcd_D6,Get_Bit(command,6));
		DIO_u8SetPinValue(Lcd_DCGroup,Lcd_D7,Get_Bit(command,7));
	//Set the E value as Low (Low CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_LOW);
#endif

}

void LCD_voidSendData(u8 data)
{
#if(LcdMode==8)
	//set the RS value as Low (Data =>RS =  1 )
	DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RsPin,DIO_u8PIN_HIGH);
	//set the RW vlaue as Low (Write  => RW = 0 )
	DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RwPin,DIO_u8PIN_LOW);
	//set the E Value as High (High CLK)
	DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_HIGH);
	//Send Data
		DIO_u8SetPortValue(Lcd_DCGroup,data);
	//Set the E value as Low (Low CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_LOW);
#elif(LcdMode==4)
	//set the RS value as Low (Data =>RS =  1 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RsPin,DIO_u8PIN_HIGH);
	//set the RW vlaue as Low (Write  => RW = 0 )
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_RwPin,DIO_u8PIN_LOW);
	//set the E Value as High (High CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_HIGH);
	//Send Data
		DIO_voidSetPortValue(Lcd_DCGroup,data);
	//Set the E value as Low (Low CLK)
		DIO_u8SetPinValue(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_LOW);

#endif

}


void LCD_voidInit()
{
#if(LcdMode==8)
	//set Direction
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_RsPin,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_RwPin,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_EPin,DIO_u8PIN_OUTPUT);
	DIO_u8SetPortDirection(Lcd_DCGroup,0xff);
	//wait for 30ms
	_delay_ms(35);
	//sent the Function set
	LCD_voidSendCommand(LCD_Set8Bit2Line5x8);
	//wait for 39us
	_delay_ms(1);
	//sent the display
	LCD_voidSendCommand(LCD_8BitCursorBlinkON);
	//wait for 39us
	_delay_ms(1);
	//sent the Clear
	LCD_voidSendCommand(LCD_8BitClearDisplay);
	//wait for 1.53ms
	_delay_ms(2);
	//sent the EntryMode
	LCD_voidSendCommand(LCD_8BitEntryMode1);
	_delay_ms(3);
	LCD_voidSendCommand(LCD_CursorBlinkOFF );

#elif(LcdMode==4)
	//set Direction
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_RsPin,Output);
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_RwPin,Output);
	DIO_u8SetPinDirection(Lcd_ControlGroup,Lcd_EPin,Output);
	DIO_u8SetPinDirection(Lcd_DCGroup,Lcd_D4,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(Lcd_DCGroup,Lcd_D5,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(Lcd_DCGroup,Lcd_D6,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(Lcd_DCGroup,Lcd_D7,DIO_u8PIN_OUTPUT);
		//wait for 30ms
		_delay_ms(35);
		//sent the Function set
		LCD_voidSendCommand(first_command_to_set);
		LCD_voidSendCommand(first_command_to_set);
		LCD_voidSendCommand(LCD_Set4Bit2Line5x8);
		//wait for 39us
		_delay_ms(1);
		//sent the display
		LCD_voidSendCommand(first_command);
		LCD_voidSendCommand(LCD_4BitCursorBlinkON);
		//wait for 39us
		_delay_ms(1);
		//sent the Clear
		LCD_voidSendCommand(first_command);
		LCD_voidSendCommand(LCD_4BitClearDisplay);
		//wait for 1.53ms
		_delay_ms(2);
		//sent the EntryMode
		LCD_voidSendCommand(first_command);
		LCD_voidSendCommand(LCD_4BitEntryMode1);
#endif
}


void LCD_voidSendString(u8 String[])
{
	u8 i = 0 ;
	while(String[i]!='\0')
	{
		LCD_voidSendData(String[i]);
		i++;
	}
}


void LCD_voidSetPos(u8 x, u8 y)
{
	u8 adress;
	if (x==first_line_pos)
	{
		adress=y+first_line_add;
	}
	else if (x==sec_line_pos)
	{
		adress=y+sec_line_add;
	}
	LCD_voidSendCommand(adress|LCD_SetAddressDDRAM);
}

void LCD_voidSpecialCharacter (u8 block_no , u8 *special, u8 x, u8 y)
{
	u8 adress;
	u8 counter;
	adress = block_no*8;
	LCD_voidSendCommand(adress|LCD_SetAddressCGRAM);
	for (counter=0; counter<8;counter++)
	{
		LCD_voidSendData(special[counter]);
	}
	//go back to DDRAM
	LCD_voidSetPos(x,y);
	LCD_voidSendData(block_no);
}

void LCD_voidClear()
{
	LCD_voidSendCommand(LCD_8BitClearDisplay);
	//wait for 1.53ms
	_delay_ms(2);
}
void LCD_voidSendNumber(s32 num) {
    s8 counter;
    u8 length = 0;
    u8 StringStart = 0;
    u8 IsNegative = 0;
    u8 str[20];

    if (num < 0) {
        num = -num;
        IsNegative = 1;
        StringStart = 1;
        str[0] = '-';
        length++;
    }

    u32 temp_num = num;
    do {
        temp_num /= 10;
        length++;
    } while (temp_num > 0);

    for (counter = length - 1; counter >= StringStart; counter--) {
        str[counter] = num % 10 + '0';
        num /= 10;
    }
    str[length] = '\0';
    LCD_voidSendString(str);
}
