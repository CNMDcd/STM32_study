/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：电机驱动(脉冲控制) 实现文件(pwm.c)
*************************/

/****
电机驱动：L298N
驱动电源接入：vcc: 5v
			stm32f407vet6 最大电压 5v

			注意，在输入电压为5v时，电机驱动无法同时支持鼓风机和电机最大功率运行
			所以，测试及演示时需为电机驱动外接电源(充电宝)

脉冲信号控制： GPIOC_Pin_0 GPIOC_Pin_1
			用以控制鼓风机(微型2006_5v鼓风机)
               GPIOC_Pin_2 GPIOC_Pin_3
			用以控制电机(风扇)

			//
			   
定时器：TIM5

****/

#include "pwm.h"

void Pwm_Init(void)
{
	
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	// TIM5 定时器使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
	// GPIO A组 使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// 引脚设置
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;   
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	// GPIO A组 进行引脚设置
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	// 给指定A组 引脚(0,1,2,3) 指定复用为定时器TIM5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); 

	// 定时器设置 1MHZ 计数1000 --> 每计数1 耗时1微秒
	// 							--> 计数1000 耗时一毫秒
	// 			--> 单次计时器 计时完成 即为 一毫秒
	TIM_TimeBaseStructure.TIM_Period 		= (1000-1);
	TIM_TimeBaseStructure.TIM_Prescaler 	= (84-1);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up; 
	// 给指定定时器TIM5 进行设置
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
	
	// 定时器输出通道设置
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1; 	
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 		= 0;  
	
	// 给指定定时器TIM5 的(OC1、OC2、OC3、OC4)四个通道 进行设置
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	
	// 给指定定时器TIM5 的(OC1、OC2、OC3、OC4)四个通道 进行使能(也叫使能预装载寄存器)
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); 
	
	// 使能TIM5 的ARR重载值寄存器
	TIM_ARRPreloadConfig(TIM5, ENABLE); 
	
	// 使能定时器5
	TIM_Cmd(TIM5, ENABLE); 
	
	// 将四通道的输出都进行关闭
	TIM_SetCompare1(TIM5, 0);
	TIM_SetCompare2(TIM5, 0); 
	TIM_SetCompare3(TIM5, 0); 
	TIM_SetCompare4(TIM5, 0); 
}
