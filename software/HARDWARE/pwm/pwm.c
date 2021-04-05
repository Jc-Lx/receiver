#include "pwm.h"

//定义全局遥控器接收数组
extern uint8_t   transport_date[8];

RCData_t RCData ;
/*
*********************************************************************************************************
*	函 数 名: void PWM_DeModulation(void)
*	功能说明: 接收数据解码成PWM宽度
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void PWM_DeModulation(void)
{
    RCData.yaw      = ((((int16_t)transport_date[0]) << 8) | transport_date[1]);
    RCData.throttle = ((((int16_t)transport_date[2]) << 8) | transport_date[3]);
    RCData.roll     = ((((int16_t)transport_date[4]) << 8) | transport_date[5]);
    RCData.pith     = ((((int16_t)transport_date[6]) << 8) | transport_date[7]);
}



