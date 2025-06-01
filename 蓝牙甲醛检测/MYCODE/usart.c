#include "usart.h"


/*********************************
引脚说明：

PA9  ---- USART1_TX(发送端)
PA10 ---- USART1_RX(接收端)

**********************************/

void Usart1_Init(int my_BaudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	//串口时钟使能，GPIOA 时钟使能。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//设置引脚复用器映射：调用 GPIO_PinAFConfig 函数。
	//PA9选择复用功能为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);	
	//PA10选择复用功能为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);	

	
	//GPIO 初始化设置：要设置模式为复用功能。
	//配置引脚9
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;   	//引脚9 10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;  	//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	
	
	USART_InitStruct.USART_BaudRate	= my_BaudRate;		//波特率
	USART_InitStruct.USART_Mode		= USART_Mode_Rx|USART_Mode_Tx; //全双工	
	USART_InitStruct.USART_Parity	= USART_Parity_No; //无校验位
	USART_InitStruct.USART_StopBits	= USART_StopBits_1; //停止位
	USART_InitStruct.USART_WordLength= USART_WordLength_8b;	//字长：1字节
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
	//串口参数初始化：设置波特率，字长，奇偶校验等参数。
	USART_Init(USART1, &USART_InitStruct);
	
	
	
	//开启中断并且初始化 NVIC，使能中断（如果需要开启串口中断才需要这个步骤）。
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;   //中断通道在stm32f4xx.h查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;  //通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	
	//配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    
	//使能串口。
	USART_Cmd(USART1, ENABLE);

}