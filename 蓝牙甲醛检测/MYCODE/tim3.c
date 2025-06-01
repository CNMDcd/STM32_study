/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺��ʱ��3 2  ʵ���ļ�(tim3.h)
*************************/ 
 
#include "tim3.h"

void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	
	//ʹ�ܶ�ʱ��ʱ�ӡ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//1ms�������--�ж�
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1); //����(ARR)���Զ�����ֵ ��1MHZ�£���1000��������ʱ1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   //84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //��Ƶ����
	//��ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1); //����(ARR)���Զ�����ֵ ��1MHZ�£���1000��������ʱ1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   //84��Ƶ 84MHZ/84 = 1MHZ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	

	
	//����ʱ���жϣ�����NVIC��
	//NVIC_InitStructure.NVIC_IRQChannel 						= TIM3_IRQn;   //�ж�ͨ����stm32f4xx.h����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x03; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x03;	//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;  //ͨ��ʹ��
	//NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn; 
	NVIC_Init(&NVIC_InitStructure);
	
	//���� TIM3_DIER  ��������ж�
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//ʹ�ܶ�ʱ����
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM2, ENABLE);
}


 
