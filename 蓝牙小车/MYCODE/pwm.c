#include "pwm.h"

/*********************************
 	四轮， 需要四对PWM信号来加以控制
	
1. PC6  TIM1_CH1 -- DCMI_D0			// 右
2. PC7  TIM1_CH2 -- DCMI_D1
3. PC8  TIM1_CH3 -- DCMI_D2
4. PC9  TIM1_CH4 -- DCMI_D3

5. PD14 TIM4_CH3 -- FSMC_D0			// 左
6. PD15 TIM4_CH4 -- FSMC_D1	

7. PA2 	TIM5_CH3 -- USART2
8. PA3  TIM5_CH4 -- USART2


2025.04.17::19:57 完成
**********************************/

void Pwm_one_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	//使能定时器3 4 5
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	//使能GPIOA C D
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	//配置引脚 
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2|GPIO_Pin_3; //引脚9
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;  	//复用模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //引脚9
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14|GPIO_Pin_15; //引脚9
	GPIO_Init(GPIOD, &GPIO_InitStructure); 


	/* Connect TIM4 pins to AF9 */  
	//PD14 15选择复用功能为定时器14
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	 


	//PWM周期是:1ms
	TIM_TimeBaseStructure.TIM_Period 		= (1000-1);
	TIM_TimeBaseStructure.TIM_Prescaler 	= (84-1);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up; //向上计数 

 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	

	//配置捕获/比较寄存器
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1; 	//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;//极性电平，这里配置为低电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能通道
	TIM_OCInitStructure.TIM_Pulse 		= 0;   //初始化CCR1的值


	//GPIO_PA
	//OC3--通道3
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	//OC4--通道4
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);

	//GPIO_PD
	//OC3--通道3
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	//OC4--通道4
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 

	//GPIO_PC
	//OC1--通道1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	//OC2--通道2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	//OC3--通道3
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	//OC4--通道4
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	
	
	//GPIO_PA
	//OC3--使能预装载寄存器
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	//OC4--使能预装载寄存器
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

	//GPIO_PC
	//OC1--使能预装载寄存器
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//OC2--使能预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//OC3--使能预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//OC4--使能预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//GPIO_PD
	//OC3--使能预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//OC4--使能预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	
	
	
	//使能ARR重载值寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_ARRPreloadConfig(TIM5, ENABLE);

	//使能定时器
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM5, ENABLE);

	// 高级定时器需开启
	//TIM_CtrlPWMOutputs(TIM8,ENABLE); //MOE 主输出使能

/*********************************
 
1. PC6  TIM3_CH3 -- DCMI_D0			// 右
2. PC7  TIM3_CH4 -- DCMI_D1
3. PC8  TIM3_CH3 -- DCMI_D2
4. PC9  TIM3_CH4 -- DCMI_D3

5. PD14 TIM4_CH3 -- FSMC_D0			// 左
6. PD15 TIM4_CH4 -- FSMC_D1	

7. PA2 	TIM5_CH3 -- USART2
8. PA3  TIM5_CH4 -- USART2
**********************************/	

	TIM_SetCompare3(TIM5,0);		// PA USART2
	TIM_SetCompare4(TIM5,0);
	
	TIM_SetCompare1(TIM3,0);		// PC DCMI
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);		// PC DCMI
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,0);		// PD FSMC
	TIM_SetCompare4(TIM4,0);		
}

void cat_start(int catSum)
{					
	catSum = 400;
	TIM_SetCompare1(TIM3,catSum);	// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,catSum);	// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,catSum);	// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,catSum);	// 右后
	TIM_SetCompare4(TIM5,0);
}

void cat_stop(void)
{
	TIM_SetCompare1(TIM3,0);	// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,0);	// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,0);	// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,0);	// 右后
	TIM_SetCompare4(TIM5,0);
}

void cat_Advance(int catSum)
{
	TIM_SetCompare1(TIM3,catSum);	// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,catSum);	// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,catSum);	// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,catSum);	// 右后
	TIM_SetCompare4(TIM5,0);
}

void cat_Backoff(int catSum)
{
	TIM_SetCompare1(TIM3,0);	// 左前
	TIM_SetCompare2(TIM3,catSum);

	TIM_SetCompare3(TIM3,0);	// 左后
	TIM_SetCompare4(TIM3,catSum);

	TIM_SetCompare3(TIM4,0); 	// 右前
	TIM_SetCompare4(TIM4,catSum);

	TIM_SetCompare3(TIM5,0);	// 右后
	TIM_SetCompare4(TIM5,catSum);	
}


void cat_AdvanceLeft(int catSum)
{
	TIM_SetCompare1(TIM3,catSum - 400);	// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,catSum - 100);	// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,catSum + 200);	// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,catSum + 200);	// 右后
	TIM_SetCompare4(TIM5,0);
}

void cat_BackoffLeft(int catSum)
{
	TIM_SetCompare1(TIM3,0);	// 左前
	TIM_SetCompare2(TIM3,catSum - 100);

	TIM_SetCompare3(TIM3,0);	// 左后
	TIM_SetCompare4(TIM3,catSum - 400);

	TIM_SetCompare3(TIM4,0);	// 右前
	TIM_SetCompare4(TIM4,catSum + 200);

	TIM_SetCompare3(TIM5,0);	// 右后
	TIM_SetCompare4(TIM5,catSum + 200);	
}

void cat_AdvanceRight(int catSum)
{
	TIM_SetCompare1(TIM3,catSum + 200);	// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,catSum + 200);	// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,catSum - 400);	// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,catSum - 100);	// 右后
	TIM_SetCompare4(TIM5,0);
}

void cat_BackoffRight(int catSum)
{
	TIM_SetCompare1(TIM3,0);	// 左前
	TIM_SetCompare2(TIM3,catSum + 200);

	TIM_SetCompare3(TIM3,0);	// 左后
	TIM_SetCompare4(TIM3,catSum + 200);

	TIM_SetCompare3(TIM4,0);	// 右前
	TIM_SetCompare4(TIM4,catSum - 100);

	TIM_SetCompare3(TIM5,0);	// 右后
	TIM_SetCompare4(TIM5,catSum - 400);	
}

void cat_Leftss(void)
{
	TIM_SetCompare1(TIM3,0);		// 左前
	TIM_SetCompare2(TIM3,800);

	TIM_SetCompare3(TIM3,900);		// 左后
	TIM_SetCompare4(TIM3,0);

	TIM_SetCompare3(TIM4,800);		// 右前
	TIM_SetCompare4(TIM4,0);

	TIM_SetCompare3(TIM5,0);		// 右后
	TIM_SetCompare4(TIM5,900);	
}

void cat_Rightss(void)
{
	TIM_SetCompare1(TIM3,800);		// 左前
	TIM_SetCompare2(TIM3,0);

	TIM_SetCompare3(TIM3,0);		// 左后
	TIM_SetCompare4(TIM3,900);

	TIM_SetCompare3(TIM4,0);		// 右前
	TIM_SetCompare4(TIM4,800);

	TIM_SetCompare3(TIM5,900);		// 右后
	TIM_SetCompare4(TIM5,0);	
}




