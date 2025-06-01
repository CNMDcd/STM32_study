#include "sr04.h"


/*********************************
引脚说明：

 TRIG（触发信号）  
 ECHO（回响信号）

PE5 做为普通输出	DCMI_D6
PE6 做为普通输入	DCMI_D7

**********************************/
void Sr04_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    
    //1、能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	//使能GEPOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5; //引脚2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//推挽模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //输出速度
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6; 		//引脚3
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 	//输入模式
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; 	//上拉
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	

    
    TIM_TimeBaseInitStruct.TIM_Prescaler    = 84-1;                  // 84分频 84MHZ/84 = 1MHZ
    TIM_TimeBaseInitStruct.TIM_Period    	= 50000-1;                        
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;    // 向上计数
    TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;            // 分频因子
    //2、初始化定时器，配置ARR,PSC
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
            
    //5,不使能定时器
    TIM_Cmd(TIM2, DISABLE);

}

//返回-1: 启动失败
//返回-2：表示接受失败
//返回其它值：表示测量成功

int Get_Sr04_Value(void)
{
	int dis, temp, t = 0;
	
	//启动信号
	//PE5输出低电平
	PEout(5) = 0;
	delay_us(8);
	//PE5输出高电平
	PEout(5) = 1;
	delay_us(20);
	//PE5输出低电平
	PEout(2) = 0;
	
	
	//设置定时器的CNT为0  
	TIM2->CNT = 0;
	
	
	t = 0;
	//PE6等待高电平到来，参考按键松开代码 
	while( PEin(6) == 0)
	{
		t++;
		delay_us(10);
		
		if(t >= 500)  //5000us未检测到高电平，则退出
		{
			return -1;
		}
	}

	//使能定时器开始计数
    TIM_Cmd(TIM2, ENABLE);
	
	
	t = 0;
	//PA3等待低电平到来，while( 读电平 == 1);
	while( PEin(6) == 1)
	{
		t++;
		delay_us(10);
		
		
		if(t >= 2900)  //等待29ms未有低电平到来，则返回
		{
			return -2;
		}		
		
	}

	//获取定时器CNT值，从而得到高电平持续时间    
	temp = TIM2->CNT;

	//关闭定时器
	TIM_Cmd(TIM2, DISABLE);

	//通过公式计算出超声波测量距离	temp是高电平持续时间
	dis = temp/58;
	
	return dis;
}
	
