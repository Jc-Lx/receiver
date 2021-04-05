#include "delay.h"
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
*	功能说明: 启动NRF24L01P接收一次数据
*	形    参：txbuf:接收数据待存放首地址
*	返 回 值: 0，接收完成；其他，错误代码
*********************************************************************************************************
*/
void HSE_SetSysClk(uint32_t RCC_PLLMul_x)
{
	ErrorStatus HSEStatus;
	RCC_DeInit();//复位RCC相关寄存器
	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);

	//等待HSE就绪并做超时处理
	HSEStatus = RCC_WaitForHSEStartUp();

	if (HSEStatus == SUCCESS)
	{
		//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//使能预取指
		//FLASH_SetLatency(FLASH_Latency_2);//设置FLASH等待周期
		/* Enable Prefetch Buffer */
    	FLASH->ACR |= FLASH_ACR_PRFTBE;

    	/* Flash 2 wait state */
    	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;   

		RCC_HCLKConfig(RCC_SYSCLK_Div1);//设置HCLK = SYSCLK时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);//设置PCLK1时钟 36MHz
		RCC_PCLK2Config(RCC_HCLK_Div1);//设置PCLK2时钟 72MHz

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x);//配置PLLCLK = HSE*RCC_PLLMul_x(2-16可选)
		RCC_PLLCmd(ENABLE);//使能PLL

		while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL稳定

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//配置系统时钟来源

		while(RCC_GetSYSCLKSource() != 0x08);//等待时钟配置成功
	}
	else
	{
		//如果HSE启动失败，用户可以在这里添加处理错误代码
	}	

}
void delay_us(uint32_t us)
{
	SysTick_Config(72);
	for (uint32_t i = 0; i < us; i++)
	{
		while(!(( SysTick->CTRL) & (1<<16)));
	}
	SysTick->CTRL &=~SysTick_CTRL_CLKSOURCE_Msk;
}
void delay_ms(uint16_t ms)
{
	SysTick_Config(72000);
	for (uint32_t i = 0; i < ms; i++)
	{
		while(!(( SysTick->CTRL) & (1<<16)));
	}
	SysTick->CTRL &=~SysTick_CTRL_CLKSOURCE_Msk;
}









































