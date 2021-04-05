#include "delay.h"
/*
*********************************************************************************************************
*	�� �� ��: uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
*	����˵��: ����NRF24L01P����һ������
*	��    �Σ�txbuf:�������ݴ�����׵�ַ
*	�� �� ֵ: 0��������ɣ��������������
*********************************************************************************************************
*/
void HSE_SetSysClk(uint32_t RCC_PLLMul_x)
{
	ErrorStatus HSEStatus;
	RCC_DeInit();//��λRCC��ؼĴ���
	//ʹ��HSE
	RCC_HSEConfig(RCC_HSE_ON);

	//�ȴ�HSE����������ʱ����
	HSEStatus = RCC_WaitForHSEStartUp();

	if (HSEStatus == SUCCESS)
	{
		//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//ʹ��Ԥȡָ
		//FLASH_SetLatency(FLASH_Latency_2);//����FLASH�ȴ�����
		/* Enable Prefetch Buffer */
    	FLASH->ACR |= FLASH_ACR_PRFTBE;

    	/* Flash 2 wait state */
    	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;   

		RCC_HCLKConfig(RCC_SYSCLK_Div1);//����HCLK = SYSCLKʱ��
		RCC_PCLK1Config(RCC_HCLK_Div2);//����PCLK1ʱ�� 36MHz
		RCC_PCLK2Config(RCC_HCLK_Div1);//����PCLK2ʱ�� 72MHz

		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x);//����PLLCLK = HSE*RCC_PLLMul_x(2-16��ѡ)
		RCC_PLLCmd(ENABLE);//ʹ��PLL

		while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//�ȴ�PLL�ȶ�

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//����ϵͳʱ����Դ

		while(RCC_GetSYSCLKSource() != 0x08);//�ȴ�ʱ�����óɹ�
	}
	else
	{
		//���HSE����ʧ�ܣ��û�������������Ӵ���������
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









































