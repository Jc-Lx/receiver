#include "key.h"

/*
*********************************************************************************************************
*	�� �� ��: void KEY_Init(void)
*	����˵��: ������ʼ������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/								    
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTBʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.5

}
/*
*********************************************************************************************************
*	�� �� ��: uint8_t KEY_Scan(uint8_t mode)
*	����˵��: ���ذ���ֵ
*	��    ��: mode:0,��֧��������;1,֧��������;
*	�� �� ֵ: 0,û���κΰ�������,1,KEY0����
*********************************************************************************************************
*/
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
	}else if(KEY0==1)key_up=1; 	    
 	return 0;// �ް�������
}
