/*
 * UART_private.h
 *
 *  Created on: Aug 3, 2024
 *      Author: Ahmed
 */

#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_

#define UDR     *((volatile u8*)  0x2C)
#define UCSRA   *((volatile u8*)  0x2B)
#define UCSRB   *((volatile u8*)  0x2A)
#define UCSRC   *((volatile u8*)  0x40)
#define UBRRH   *((volatile u8*)  0x40)
#define UBRRL   *((volatile u8*)  0x29)

//UDR 2 contents
#define UDR_TXB  1
#define UDR_RXB  0



//UCSRA Register bits
#define UCSRA_RXC  7
#define UCSRA_TXC  6
#define UCSRA_UDRE 5
#define UCSRA_FE   4
#define UCSRA_DOR  3
#define UCSRA_PE   2
#define UCSRA_U2X  1
#define UCSRA_MPCM 0




//UCSRB Register bits
#define UCSRB_RXCIE  7
#define UCSRB_TXCIE  6
#define UCSRB_UDRIE  5
#define UCSRB_RXEN   4
#define UCSRB_TXEN   3
#define UCSRB_UCSZ2  2
#define UCSRB_RXB8   1
#define UCSRB_TXB8   0


//UCSRC Register bits
#define UCSRC_URSEL 7
#define UCSRC_UMSEL 6
#define UCSRC_UPM1  5
#define UCSRC_UPM0  4
#define UCSRC_USBS  3
#define UCSRC_UCSZ1 2
#define UCSRC_UCSZ0 1
#define UCSRC_UCPOL 0


// UBRRH bits

#define  UBRRH_URSEL 7






#endif /* UART_PRIVATE_H_ */
