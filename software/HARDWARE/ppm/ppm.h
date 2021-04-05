#ifndef __PPM_H
#define __PPM_H

#include "sys.h"


#define PPM_NUM  12//PPM通道数量

/* 寄存器操作IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //??????		
#define digitalLo(p,i)		 {p->BRR=i;}	 //?????
#define digitalToggle(p,i)   {p->ODR ^=i;} //??????

/* PPMIO定义 */
#define PPM_TOGGLE		    digitalToggle(GPIOA,GPIO_Pin_6)
#define PPM_OFF			    digitalHi(GPIOA,GPIO_Pin_6)
#define PPM_ON				digitalLo(GPIOA,GPIO_Pin_6)	

void PPM_Init( void );
void RCToPPM(void);

#endif


