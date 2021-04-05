#ifndef __DELAY_H
#define __DELAY_H 
			   
#include "stm32f10x.h"
//#include "core_cm3.h" 

	 
void delay_ms(uint16_t ms);
void delay_us(uint32_t us);

void HSE_SetSysClk(uint32_t RCC_PLLMul_x);

#endif





























