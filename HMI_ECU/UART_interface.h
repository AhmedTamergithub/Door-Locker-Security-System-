/*
 * UART_interface.h
 *
 *  Created on: Aug 3, 2024
 *      Author: Ahmed
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

typedef u8  UART_BitData;
typedef u8 UART_Parity;
typedef u8 UART_StopBit;
typedef u32 UART_BaudRate;

typedef struct{
     UART_BitData bit_data;
    UART_Parity parity;
    UART_StopBit stop_bit;
    UART_BaudRate baud_rate;
}UART_ConfigType;


void UART_init(const UART_ConfigType * Config_Ptr);
void UART_voidsend(u8 Copy_u8data);
u8 UART_voidreceive(void);
void UART_clearbuffer(void);


#endif /* UART_INTERFACE_H_ */
