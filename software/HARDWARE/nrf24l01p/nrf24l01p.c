#include "nrf24l01P.h"

    
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
/*
*********************************************************************************************************
*	函 数 名: static void EXTI9_Init(void)
*	功能说明: 外部中断线EXTI9初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
// static void EXTI9_Init(void)
// {
// 	EXTI_InitTypeDef EXTI9;
// 	NVIC_InitTypeDef NVIC_Structure;
	
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	
// 	EXTI9.EXTI_Line=EXTI_Line9;
// 	EXTI9.EXTI_LineCmd=ENABLE;
// 	EXTI9.EXTI_Mode=EXTI_Mode_Interrupt;
// 	EXTI9.EXTI_Trigger=EXTI_Trigger_Falling;
// 	EXTI_Init(&EXTI9);
	
// 	NVIC_Structure.NVIC_IRQChannel=EXTI9_5_IRQn;
// 	NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
// 	NVIC_Structure.NVIC_IRQChannelPreemptionPriority=10;
// 	NVIC_Structure.NVIC_IRQChannelSubPriority=0;
// 	NVIC_Init(&NVIC_Structure);
// }

/*
*********************************************************************************************************
*	函 数 名: static void NRF_GPIO_Config(void)
*	功能说明: NRF相关GPIO初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
static void NRF_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,A端口时钟

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_12;	//PA8,PB12 推挽 	  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化指定IO
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA9 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_9);

	GPIO_ResetBits(GPIOA,GPIO_Pin_8);//PA8上拉
    GPIO_ResetBits(GPIOB,GPIO_Pin_12);//PB12上拉

}

/*
*********************************************************************************************************
*	函 数 名: static void NRF_SPI_Config(void)
*	功能说明: NRFSPI初始化
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
static void NRF_SPI_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI2_Init();    		//初始化SPI
	SPI_Cmd(SPI2, DISABLE); // SPI外设不使能

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主机
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
}

/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_Init(void)
*	功能说明: NRF初始化函数
*	形    参:
*	返 回 值: 
*********************************************************************************************************
*/
void NRF24L01P_Init(void)
{ 	
	NRF_GPIO_Config();
	// EXTI9_Init();	 		//NRF中断引脚设置
	NRF_SPI_Config();			 
	NRF24L01P_CE=0; 			//使能24L01p
	NRF24L01P_CSN=1;			//SPI片选取消 	 		 	 
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Check(void)
*	功能说明: 检测24L01p是否存在
*	形    参:
*	返 回 值: 0,成功;1,失败
*********************************************************************************************************
*/
uint8_t NRF24L01P_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01p的最大SPI时钟为10Mhz）   	 
	NRF24L01P_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01P_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01p错误	
	return 0;		 //检测到24L01p
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Write_Reg(uint8_t reg,uint8_t value)
*	功能说明: SPI写NRF寄存器
*	形    参: reg:指定寄存器地址,value:写入的值
*	返 回 值: 返回状态值
*********************************************************************************************************
*/
uint8_t NRF24L01P_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
   	NRF24L01P_CSN=0;                 //使能SPI传输
  	status =SPI2_ReadWriteByte(reg);//发送寄存器号 
  	SPI2_ReadWriteByte(value);      //写入寄存器的值
  	NRF24L01P_CSN=1;                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Read_Reg(uint8_t reg)
*	功能说明: SPI读NRF寄存器
*	形    参: reg:指定寄存器地址
*	返 回 值: 返回状态值
*********************************************************************************************************
*/
uint8_t NRF24L01P_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF24L01P_CSN = 0;          //使能SPI传输		
  	SPI2_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI2_ReadWriteByte(0XFF);//读取寄存器内容
  	NRF24L01P_CSN = 1;          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
*	功能说明: 在指定位置读出指定长度的数据
*	形    参: reg:寄存器(位置),*pBuf:数据指针,len:数据长度
*	返 回 值: 此次读到的状态寄存器值 
*********************************************************************************************************
*/	
uint8_t NRF24L01P_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	       
  	NRF24L01P_CSN = 0;           //使能SPI传输
  	status=SPI2_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)pBuf[uint8_t_ctr]=SPI2_ReadWriteByte(0XFF);//读出数据
  	NRF24L01P_CSN=1;       //关闭SPI传输
  	return status;        //返回读到的状态值
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
*	功能说明: 在指定位置写指定长度的数据
*	形    参: reg:寄存器(位置),*pBuf:数据指针,len:数据长度
*	返 回 值: 此次读到的状态寄存器值 
*********************************************************************************************************
*/
uint8_t NRF24L01P_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
 	NRF24L01P_CSN = 0;          //使能SPI传输
  	status = SPI2_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)SPI2_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF24L01P_CSN = 1;       //关闭SPI传输
  	return status;          //返回读到的状态值
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_TxPacket(uint8_t *txbuf)
*	功能说明: 启动NRF24L01P发送一次数据
*	形    参: txbuf:待发送数据首地址
*	返 回 值: 发送完成状况 
*********************************************************************************************************
*/				   
uint8_t NRF24L01P_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi速度为9Mhz（24L01p的最大SPI时钟为10Mhz）   
	NRF24L01P_CE=0;
  	NRF24L01P_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01P_CE=1;//启动发送	   
	while(NRF24L01P_IRQ!=0);//等待发送完成
	sta=NRF24L01P_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01P_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01P_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
*	功能说明: 启动NRF24L01P接收一次数据
*	形    参: txbuf:接收数据存放首地址
*	返 回 值: 0,接收完成;其他,错误代码 
*********************************************************************************************************
*/
uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	//SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01p的最大SPI时钟为10Mhz）   
	sta=NRF24L01P_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01P_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01P_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01P_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}

/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_RX_Mode(void)
*	功能说明: 该函数初始化NRF24L01P到RX模式,设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
*	形    参: 
*	返 回 值: 
*	备    注: 当CE变高后,即进入RX模式,并可以接收数据了
*********************************************************************************************************
*/					    		   
void NRF24L01P_RX_Mode(void)
{
	NRF24L01P_CE=0;	  
  	NRF24L01P_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
	NRF24L01P_Write_Reg(NRF_WRITE_REG+STATUS,0xff); //清除所有的中断标志位
  	NRF24L01P_CE = 1; //CE为高,进入接收模式
	delay_us(130);        //开始进入接收模式，等待130us 
}

/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_TX_Mode(void)
*	功能说明: 该函数初始化NRF24L01P到TX模式
*	形    参: 
*	返 回 值: 
*	备    注: 设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR,PWR_UP,CRC使能
			  当CE变高后,即进入RX模式,并可以接收数据了,CE为高大于10us,则启动发送.
*********************************************************************************************************
*/						 	 
void NRF24L01P_TX_Mode(void)
{														 
	NRF24L01P_CE=0;	    
  	NRF24L01P_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01P_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01P_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01P_CE=1;//CE为高,10us后启动发送
	delay_us(10);         //如果是连续发射，在初始化阶段就要拉高CE10us以上，一个一个包发送等到发送在拉高

}

/*
*********************************************************************************************************
*	函 数 名: void EXTI9_5_IRQHandler(void)
*	功能说明: 接收中断函数
*	形    参: 
*	返 回 值: 
*	备    注: 
*********************************************************************************************************
*/
// void EXTI9_5_IRQHandler(void)
// {                        
// 	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
// 	{
// 		NRF24L01P_RxPacket(transport_date);
// 		PPM_Date[0] = 1000;
// 		PPM_Date[1] = ((((int16_t)transport_date[0]) << 8) | transport_date[1]) - 1000;
// 		PPM_Date[2] = 1000;
//     	PPM_Date[3] = ((((int16_t)transport_date[2]) << 8) | transport_date[3]) - 1000;
// 		PPM_Date[4] = 1000;
//     	PPM_Date[5] = ((((int16_t)transport_date[4]) << 8) | transport_date[5]) - 1000;
// 		PPM_Date[6] = 1000;
//     	PPM_Date[7] = ((((int16_t)transport_date[6]) << 8) | transport_date[7]) - 1000;
// 		PPM_Date[8] = 500;
// 		PPM_Date[9] = 4500;
// 		NRF_RX_Flag ++;			
// 		LED0 = !LED0;
// 		EXTI_ClearITPendingBit(EXTI_Line9);
// 	}
// }

