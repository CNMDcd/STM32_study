
#include "delay.h"

u32 my_us = 21;  	 
u32 my_ms = 21000;   

void Delay_Init(void)
{ 
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

 
void delay_us(int nus) 
{
	u32 temp;
	
	 
	SysTick->LOAD = my_us*nus - 1;
	
	SysTick->VAL = 0x00;
	
	 
	SysTick->CTRL |= (0x01<<0);
	
	 
	do
	{
		temp = SysTick->CTRL; 
	}while((temp & (0x01<<0)) && !(temp & (0x01<<16)));
	
	 
	SysTick->CTRL &= ~(0x01<<0);
}


 
void delay_ms(int nms)  
{
	u32 temp;
	
	 
	SysTick->LOAD = my_ms*nms - 1;
	
	SysTick->VAL = 0x00;
	
 
	SysTick->CTRL |= (0x01<<0);
 
	do
	{
		temp = SysTick->CTRL;  
	}while((temp & (0x01<<0)) && !(temp & (0x01<<16)));
	
	 
	SysTick->CTRL &= ~(0x01<<0);
}

void delay_s(int ns)
{
	for(int i=0; i<ns; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
