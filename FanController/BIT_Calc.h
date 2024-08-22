#ifndef BIT_CALC_H
#define BIT_CALC_H

#define Set_Bit(Var, Bit_No) (Var) |= (1 << (Bit_No))

#define Clr_Bit(Var, Bit_No) (Var) &= ~(1 << (Bit_No))

#define Tog_Bit(Var, Bit_No) (Var) ^= (1 << (Bit_No))

#define Get_Bit(Var, Bit_No) (((Var) >> (Bit_No)) & (1))

#endif