/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：超声波 实现文件(hcsr04.c)
*************************/

#include "hcsr04.h"
#include "delay.h"
//#include "sys.h"

/****
超声波模块：HCSR-04
	信号收发： GPIOB_Pin_8 GPIOB_Pin_9;

定时器：TIM4 

通道：OC3 OC4
****/

void Hcsr04_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8; 
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9; 
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	

    
    TIM_TimeBaseInitStruct.TIM_Period    	= 50000-1;                        
    TIM_TimeBaseInitStruct.TIM_Prescaler    = 84-1;                  // 84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;    // 向上计数
    TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;            // 分频因子
    //2、初始化定时器，配置ARR,PSC
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
            
    //5,不使能定时器
    TIM_Cmd(TIM4, DISABLE);

}

int Get_Hcsr04_Value(void)
{
	int dis, temp, t = 0;
	
	//启动信号 
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	delay_us(8); 
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	
	//设置定时器的CNT为0  
	TIM4->CNT = 0;
	
	
	t = 0;
	while( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
	{
		t++;
		delay_us(10);
		
		if(t >= 500)  //5000us未检测到高电平，则退出
		{
			return 0;
		}
	}

	//使能定时器开始计数
    TIM_Cmd(TIM4, ENABLE);
	
	
	t = 0;
	//PA3等待低电平到来，while( 读电平 == 1);
	while( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 1)
	{
		t++;
		delay_us(10);
		
		
		if(t >= 2900)  //等待29ms未有低电平到来，则返回
		{
			return 0;
		}		
		
	}

	//获取定时器CNT值，从而得到高电平持续时间    
	temp = TIM4->CNT;

	//关闭定时器
	TIM_Cmd(TIM4, DISABLE);

	//通过公式计算出超声波测量距离	temp是高电平持续时间
	dis = temp/58;
	
	return dis;
}
	
