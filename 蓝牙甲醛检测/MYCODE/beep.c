/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺������ ʵ���ļ�(beep.c)
*************************/

#include "beep.h"

/****
������
	�ź��շ���  
 
****/

void beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	
	// ����ʱ��ʹ�ܣ�GPIOʱ��ʹ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	 
	
	// GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù���
	// ��������10��11

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1;	// ����10 11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;				// ����ģʽ
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
}

