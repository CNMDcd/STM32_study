//	PB7		DCMI_VSYNC

#include "SG90.h"

#define SG PBout(7)

void sss(int sum)
{
	for( int i = 0; i < 127 * sum; i++);
}

void sg90_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	/* GPIOB时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					//第8根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//设置为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式，增加驱动电流
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 

}

void sg90_up(int sum) 
{
	//printf("sg90:%d\r\n",sum);
	if(sum==0)
	{
		SG = 1;
		delay_us(500);
		
		SG = 0;
		delay_us(500);
		delay_ms(19);
	}

	if(sum==45)
	{
		SG = 1;
		delay_ms(1);
		
		SG = 0;
		delay_ms(19); 
	}


	if(sum==90)
	{
		SG = 1;
		//delay_ms(1);
		//delay_us(500);
		sss(1500);
		
		
		SG = 0;
		delay_ms(18);
		delay_us(500);
	}
	
	if(sum==135)
	{
		SG = 1;
		delay_ms(2);
		
		SG = 0;
		delay_ms(18);
	}	
	
	if(sum==180)
	{
		SG = 1;
		delay_ms(2);
		delay_us(500);
		
		SG = 0;
		delay_ms(17);
		delay_us(500);
	}		
}
 
