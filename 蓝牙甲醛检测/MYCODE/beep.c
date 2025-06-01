/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：蜂鸣器 实现文件(beep.c)
*************************/

#include "beep.h"

/****
蜂鸣器
	信号收发：  
 
****/

void beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	
	// 串口时钟使能，GPIO时钟使能 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	 
	
	// GPIO 初始化设置：要设置模式为复用功能
	// 配置引脚10、11

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1;	// 引脚10 11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;				// 复用模式
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
}

