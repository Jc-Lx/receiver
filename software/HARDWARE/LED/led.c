#include "led.h"


/*
*********************************************************************************************************
*	函 数 名: void LED_Init(void)
*	功能说明: LED IO初始化
*	形    参:
*	返 回 值:
*********************************************************************************************************
*/
void LED_Init(void)
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.6 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.6
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);						 //PB.6 输出低
}
 
