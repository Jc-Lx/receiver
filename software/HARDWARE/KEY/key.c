#include "key.h"

/*
*********************************************************************************************************
*	函 数 名: void KEY_Init(void)
*	功能说明: 按键初始化函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/								    
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.5

}
/*
*********************************************************************************************************
*	函 数 名: uint8_t KEY_Scan(uint8_t mode)
*	功能说明: 返回按键值
*	形    参: mode:0,不支持连续按;1,支持连续按;
*	返 回 值: 0,没有任何按键按下,1,KEY0按下
*********************************************************************************************************
*/
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
	}else if(KEY0==1)key_up=1; 	    
 	return 0;// 无按键按下
}
