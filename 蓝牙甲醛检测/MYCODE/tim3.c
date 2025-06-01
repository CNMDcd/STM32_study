/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：定时器3 2  实现文件(tim3.h)
*************************/ 
 
#include "tim3.h"

void Tim3_Init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	
	//使能定时器时钟。
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//1ms产生溢出--中断
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1); //周期(ARR)是自动重载值 在1MHZ下，计1000个数，用时1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   //84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //分频因子
	//初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1); //周期(ARR)是自动重载值 在1MHZ下，计1000个数，用时1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1);   //84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	

	
	//启定时器中断，配置NVIC。
	//NVIC_InitStructure.NVIC_IRQChannel 						= TIM3_IRQn;   //中断通道在stm32f4xx.h查找
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x03; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x03;	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;  //通道使能
	//NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn; 
	NVIC_Init(&NVIC_InitStructure);
	
	//设置 TIM3_DIER  允许更新中断
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//使能定时器。
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM2, ENABLE);
}


 
