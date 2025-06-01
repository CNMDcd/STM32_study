/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺�������(�������) ʵ���ļ�(pwm.c)
*************************/

/****
���������L298N
������Դ���룺vcc: 5v
			stm32f407vet6 ����ѹ 5v

			ע�⣬�������ѹΪ5vʱ����������޷�ͬʱ֧�ֹķ���͵�����������
			���ԣ����Լ���ʾʱ��Ϊ���������ӵ�Դ(��籦)

�����źſ��ƣ� GPIOC_Pin_0 GPIOC_Pin_1
			���Կ��ƹķ��(΢��2006_5v�ķ��)
               GPIOC_Pin_2 GPIOC_Pin_3
			���Կ��Ƶ��(����)

			//
			   
��ʱ����TIM5

****/

#include "pwm.h"

void Pwm_Init(void)
{
	
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	// TIM5 ��ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
	// GPIO A�� ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// ��������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;   
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	// GPIO A�� ������������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// ��ָ��A�� ����(0,1,2,3) ָ������Ϊ��ʱ��TIM5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); 

	// ��ʱ������ 1MHZ ����1000 --> ÿ����1 ��ʱ1΢��
	// 							--> ����1000 ��ʱһ����
	// 			--> ���μ�ʱ�� ��ʱ��� ��Ϊ һ����
	TIM_TimeBaseStructure.TIM_Period 		= (1000-1);
	TIM_TimeBaseStructure.TIM_Prescaler 	= (84-1);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up; 
	// ��ָ����ʱ��TIM5 ��������
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
	
	// ��ʱ�����ͨ������
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1; 	
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 		= 0;  
	
	// ��ָ����ʱ��TIM5 ��(OC1��OC2��OC3��OC4)�ĸ�ͨ�� ��������
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	
	// ��ָ����ʱ��TIM5 ��(OC1��OC2��OC3��OC4)�ĸ�ͨ�� ����ʹ��(Ҳ��ʹ��Ԥװ�ؼĴ���)
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	
	// ʹ��TIM5 ��ARR����ֵ�Ĵ���
	TIM_ARRPreloadConfig(TIM5, ENABLE); 
	
	// ʹ�ܶ�ʱ��5
	TIM_Cmd(TIM5, ENABLE); 
	
	// ����ͨ������������йر�
	TIM_SetCompare1(TIM5, 0);
	TIM_SetCompare2(TIM5, 0); 
	TIM_SetCompare3(TIM5, 0); 
	TIM_SetCompare4(TIM5, 0); 
}
