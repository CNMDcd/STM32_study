/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺���(����)  ʵ���ļ�(led.c)
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
