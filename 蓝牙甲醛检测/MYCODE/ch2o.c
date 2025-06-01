/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：甲醛检测 实现文件(ch2o.c)
*************************/

#include "ch2o.h"

/****
甲醛检测模块：JW01
	信号收发： GPIOB_Pin_10  GPIOA_Pin_11  
	ZE08-CH2O 暂停使用
	SGP30 参考 未使用
	JW01 使用
	GY- SGP30 参考 未使用

串口：USART3
****/
	    
u8 flag = 0;  		// 数据是否接受完毕
u8 count = 0;		// 数据接收位
u8 data[9] = {0};	// 接受完整数据
u8 rxdata[9] = {0};	// 存底数据

float ch2o_init;
int co2_init;
float tvoc_init;

void Ch2o_Init(void)
{  	 
  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	// 串口时钟使能，GPIO时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// 设置引脚复用器映射：调用 GPIO_PinAFConfig 函数
	// PB10选择复用功能为USART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	// PB11选择复用功能为USART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// GPIO 初始化设置：要设置模式为复用功能
	// 配置引脚10、11

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10|GPIO_Pin_11;	// 引脚10 11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;				// 复用模式
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	
	USART_InitStruct.USART_BaudRate				= 9600;					// 波特率
	USART_InitStruct.USART_Mode					= USART_Mode_Rx|USART_Mode_Tx;	// 全双工
	USART_InitStruct.USART_Parity				= USART_Parity_No;				// 无校验位
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;				// 停止位
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;			// 字长:1字节
	USART_InitStruct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;// 无硬件控制流
	// 串口参数初始化：设置波特率，字长，奇偶校验等参数。
	USART_Init(USART3, &USART_InitStruct);
	
	// 开启中断并且初始化 NVIC，使能中断（如果需要开启串口中断才需要这个步骤）。		
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;	// 中断通道在stm32f4xx.h查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;			// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x02;			// 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		// 通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	// 配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	// 使能串口
	USART_Cmd(USART3, ENABLE);
	//printf("hello\r\n");
}



// 串口3
void USART3_IRQHandler(void)
{ 
	//判断标志位是否置1
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		//printf("hello\r\n");
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// 接收数据,每次只接收一个字符
		data[count] = USART_ReceiveData(USART3);
		 
//		printf("data[%d] = %d\r\n",count,data[count]);
//		if(data[count] == 228)
//		{
//			if(data[count - 1] == 44)
//			{
//				data[0] = 44;
//				data[1] = 228;
//				count  = 1;
//			} 
//		} 
		// 0 1 2 3 4 5 6 7 8 
		count += 1;
		
		if(  count == 9)
		{
			// 表示一帧数据接收完成 
			for(int i=0; i<count-1; i++)
			{
				// 将数据从缓冲区存放到接收数组中
				rxdata[i] = data[i];
			}
			// 清空数据接收缓冲区
			memset(data, 0, sizeof(data));
			flag = 1;    // 表示帧数据接收完成
			count = 0;   // 下一帧数据从g_buffer[0]开始接收数据 
		}
	}
}


u8 CheckSum()
{
	u8 add = 0x00;
	if(flag == 1)
	{
		
		for(int i = 0; i < 5; i++)
			add += rxdata[i];
		
		co2_init = (rxdata[6]*254 + rxdata[7]);
		tvoc_init = (rxdata[2]*254 + rxdata[3]) * 0.001;
		ch2o_init = (rxdata[4]*254 + rxdata[5]) * 0.001;
		//printf("甲醛：%.4fmg/m3\r\n",ch2o_init);  
		//printf("CO2：%.2fmg/m3\r\n",co2_init);  
		//printf("TVOC：%.4fmg/m3\r\n",tvoc_init); 
		flag = 0;
	}
	
	if(add == data[8])
		return 0;
	else
		return 1;
}


float ch2oInit()
{
	 
	//if(CheckSum() == 1)
	//{
		//ch2o_init = (rxdata[4]*254 + rxdata[5]) * 0.001;
		return ch2o_init;
	//}
	//else
		return 0; 
}

int co2Init()
{
	 
	//if(CheckSum() == 1)
	//{
		//co2_init = (rxdata[6]*254 + rxdata[7]) * 0.001;
		return co2_init;
	//}
	//else
	//	return 0; 
}

float tvocInit()
{
	 
	//if(CheckSum() == 1)
	//{
		//tvoc_init = (rxdata[2]*254 + rxdata[3]) * 0.001;
		return tvoc_init;
	//}
	//else
	//	return 0; 
}

