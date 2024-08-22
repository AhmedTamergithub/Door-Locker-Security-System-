/*
 * ADC_interface.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Ahmed
 */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

typedef u8 ADC_ReferenceVolatge;
typedef u8 ADC_Prescaler;
typedef struct{
ADC_ReferenceVolatge ref_volt;
ADC_Prescaler prescaler;
}ADC_ConfigType;

void ADC_voidInit(void);
u8 ADC_u8GetChannelReading(u8 Copy_u8Channel);

#endif /* ADC_INTERFACE_H_ */
