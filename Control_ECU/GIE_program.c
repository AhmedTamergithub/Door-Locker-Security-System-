/*
 * GIE_program.c
 *
 *  Created on: Aug 9, 2024
 *      Author: Ahmed
 */



#include "BIT_Calc.h"
#include "STD_Types.h"
#include "GIE_private.h"
#include "GIE_interface.h"

void GIE_voidEnable(void){
	Set_Bit(SREG,SREG_I);
}
void GIE_voidDisable(void){
	Clr_Bit(SREG,SREG_I);
}
