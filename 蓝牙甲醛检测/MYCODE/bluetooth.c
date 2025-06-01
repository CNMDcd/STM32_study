/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：蓝牙 实现文件(bluetooth.c)
*************************/

/****
蓝牙模块：HC-06
	信号收发： GPIOA_Pin_9 GPIOA_Pin_10;

串口：USART1
****/

#include "bluetooth.h"

void Bluetooth_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);	

	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;   	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	
	
	USART_InitStruct.USART_BaudRate	= 9600;		
	USART_InitStruct.USART_Mode		= USART_Mode_Rx|USART_Mode_Tx; 	
	USART_InitStruct.USART_Parity	= USART_Parity_No; 
	USART_InitStruct.USART_StopBits	= USART_StopBits_1; 
	USART_InitStruct.USART_WordLength= USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x02;	
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    
	USART_Cmd(USART1, ENABLE);
}
