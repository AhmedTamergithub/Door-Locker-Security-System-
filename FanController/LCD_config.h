/*
 * LCD_config.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Ahmed
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#include "GPIO_interface.h"
#define Lcd_ControlGroup        DIO_u8PORTD
#define Lcd_RsPin				DIO_u8PIN0
#define Lcd_RwPin				DIO_u8PIN1
#define Lcd_EPin				DIO_u8PIN2
#define Lcd_DCGroup             DIO_u8PORTC
#define LcdMode                 8
#if(LcdMode==4)
#define Lcd_D4 DIO_Pin4
#define Lcd_D5 DIO_Pin5
#define Lcd_D6 DIO_Pin6
#define Lcd_D7 DIO_Pin7
#endif


#endif /* LCD_CONFIG_H_ */
