#include "sys.h"
 
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
/*
*********************************************************************************************************
*	函 数 名: void SYS_Init(void)
*	功能说明: 系统硬件初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
void SYS_Init(void)
{
	HSE_SetSysClk(RCC_PLLMul_9);	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	USART_Config();
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	NRF24L01P_Init();    //初始化NRF24L01P
	PPM_Init();
}


