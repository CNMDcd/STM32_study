/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：点灯(检验)  实现文件(led.c)
*************************/
 
#include "led.h"

/***
    GPIOA_Pin_6;
***/

void Led_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6; 
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
}
