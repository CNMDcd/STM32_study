#include "led.h"


/*********************************
引脚说明：

LED0连接在PF9
PF9输出低电平（0），灯亮；PF9输出高电平（1），灯灭；

LED0--PF9
LED1--PF10
LED2--PE13
LED3--PE14


**********************************/


void Led_Init(void)
{
	
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//使能GEPOE GPIOF时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_10; //引脚9 10
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13|GPIO_Pin_14; //引脚13 14
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//灭所有的灯
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);   //引脚置1
	GPIO_SetBits(GPIOF, GPIO_Pin_10);   //引脚置1
	GPIO_SetBits(GPIOE, GPIO_Pin_13);   //引脚置1
	GPIO_SetBits(GPIOE, GPIO_Pin_14);   //引脚置1

}