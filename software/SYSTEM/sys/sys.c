#include "sys.h"
 
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
/*
*********************************************************************************************************
*	�� �� ��: void SYS_Init(void)
*	����˵��: ϵͳӲ����ʼ��
*	��    ��:
*	�� �� ֵ: 
*********************************************************************************************************
*/
void SYS_Init(void)
{
	HSE_SetSysClk(RCC_PLLMul_9);	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART_Config();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	NRF24L01P_Init();    //��ʼ��NRF24L01P
	PPM_Init();
}


