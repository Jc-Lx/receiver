#include "ppm.h"

/*
*********************************************************************************************************
*	函 数 名: static void PPM_GPIO_Config( void )
*	功能说明: PPM引脚初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
static void PPM_GPIO_Config( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: static void PPM_TIM2_Config( void )
*	功能说明: PPM定时器初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
static void PPM_TIM2_Config( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义定时器时基初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;//定义中断设置结构体
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启定时器TIM2时钟

    TIM_TimeBaseStructure.TIM_Period = 500;//自动重装载寄存器的值
    TIM_TimeBaseStructure.TIM_Prescaler = 71;//驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器计数模式，设置为向上计数
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化定时器
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_ARRPreloadConfig(TIM2,DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,ENABLE);//使能计数器
    PPM_OFF;
}

/*
*********************************************************************************************************
*	函 数 名: void PPM_Init( void )
*	功能说明: PPM初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
void PPM_Init( void )
{
    PPM_GPIO_Config();
    PPM_TIM2_Config();
}

//PPM数组中的个数，这里是4通道1+4*2+1
uint16_t PPM_Index = 0;
uint16_t PPM_Date[PPM_NUM] = {1000,500,1000,500,1000,500,1000,500,1000,500,500,4500};
extern uint16_t RC_Buf[PPM_NUM];
/*
*********************************************************************************************************
*	函 数 名: void TIM2_IRQHandler(void)
*	功能说明: PPM中断处理函数
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        //printf("\r\n  %d  \r\n",PPM_Date[PPM_Index]);
        TIM2->ARR = PPM_Date[PPM_Index]-1;
        //printf("\r\n  %d  \r\n",TIM2->ARR);
        PPM_TOGGLE;
        //printf("\r\n  %d  \r\n",PPM_Index);
		PPM_Index ++;
		if ( PPM_Index >= PPM_NUM )
		{
			PPM_Index = 0;           
		}
        //printf("\r\n yaw= %d us   throttle= %d us   roll= %d us   pith= %d us \r\n",PPM_Date[2],PPM_Date[4],PPM_Date[6],PPM_Date[8]);	        
    }    
}

