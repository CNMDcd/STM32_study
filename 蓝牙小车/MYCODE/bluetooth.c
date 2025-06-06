
/****
����ģ�飺HC-06
	PB10  USART3_TX
	PB11  USART3_RX

���ڣ�USART3
****/

#include "bluetooth.h"

void Bluetooth_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);	

	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10|GPIO_Pin_11;   	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
	
	USART_InitStruct.USART_BaudRate	= 9600;		
	USART_InitStruct.USART_Mode		= USART_Mode_Rx|USART_Mode_Tx; 	
	USART_InitStruct.USART_Parity	= USART_Parity_No; 
	USART_InitStruct.USART_StopBits	= USART_StopBits_1; 
	USART_InitStruct.USART_WordLength= USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStruct);
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x02;	
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
	USART_Cmd(USART3, ENABLE);
}
