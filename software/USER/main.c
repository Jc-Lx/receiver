#include "sys.h"

extern uint16_t PPM_Date[PPM_NUM];

//定义全局遥控器接收数组
uint8_t   transport_date[8];
uint16_t RC_Buf[PPM_NUM] = {1000,500,1000,500,1000,500,1000,500,1000,500,500,4500};

uint8_t conectionflag = 0;//通讯连接标志位,0未连接,1连接.

/*
*********************************************************************************************************
*	函 数 名: int main(void)
*	功能说明: 系统主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{	
    SYS_Init();
	while(NRF24L01P_Check())
	{
		printf("\r\n    nrf dont work!    \r\n");
		LED0=!LED0;
		delay_ms(500);
	}
	NRF24L01P_RX_Mode();
	IWDG_Config(64,157);//分频64 时基1.6ms 复位周期 251.2ms
	while(1)
	{
		uint8_t cnt;

		IWDG_Feed();//喂狗
		
		if (NRF24L01P_RxPacket(transport_date) == 0)
		{
			cnt = 0;
			RC_Buf[0] = 1000;//硬件调试发现,数组如果不一次性全部赋值会出现野指针现象
			RC_Buf[1] = ((((int16_t)transport_date[0]) << 8) | transport_date[1]) - 1000;
			RC_Buf[2] = 1000;
   			RC_Buf[3] = ((((int16_t)transport_date[2]) << 8) | transport_date[3]) - 1000;
			RC_Buf[4] = 1000;
   			RC_Buf[5] = ((((int16_t)transport_date[4]) << 8) | transport_date[5]) - 1000;
			RC_Buf[6] = 1000;
   			RC_Buf[7] = ((((int16_t)transport_date[6]) << 8) | transport_date[7]) - 1000;
			RC_Buf[8] = 1000;
			RC_Buf[9] = 500;
			RC_Buf[10] = 500;
			RC_Buf[11] = 4500;
			LED0 = !LED0;
			for (uint8_t i = 0; i < 12; i++)
			{
				PPM_Date[i] = RC_Buf[i];
			}
			conectionflag = 1;			
		}
		else if(cnt >= 200)
		{
			cnt = 0;
			conectionflag = 0;		
		}
		if(conectionflag == 0)
		{
			for (uint8_t i = 0; i < 12; i++)
			{
				PPM_Date[i] = 2000;//通讯中断输出4000脉宽
			}
		}
		cnt ++;
		printf("\r\n    %d    \r\n",conectionflag);
		printf("\r\n   %d  %d   %d   %d   %d   %d",PPM_Date[0],PPM_Date[1],PPM_Date[3],PPM_Date[5],PPM_Date[7],PPM_Date[11]);		
	}
		 
}

