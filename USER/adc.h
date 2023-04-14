#ifndef __ADC_H
#define __ADC_H
#include "air32f10x.h"
// #include "sys.h"

void Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
float Get_Voltage(uint8_t ch, uint8_t times);
#endif
