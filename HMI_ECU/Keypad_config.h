/*
 * Keypad_config.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Ahmed
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define KeypadGroup DIO_u8PORTC
#define Row      4
#define Column   4
#define Pressed  0
#define Unpressed 255
#define KEYPAD_ARR1 \
    { \
        {7, 8, 9, 0}, \
        {4, 5, 6, 0}, \
        {1, 2, 3, 0}, \
        {13, 0, 15, 0} \
    }
#define KEYPAD_ARR2 \
    { \
        {'7','8','9','/'}, \
        {'4','5','6','*'}, \
        {'1','2','3','-'}, \
        {'R','0','C','+'} \
    }


#endif /* KEYPAD_CONFIG_H_ */
