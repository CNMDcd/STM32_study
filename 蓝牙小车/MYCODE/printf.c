#include "printf.h"

#pragma import(__use_no_semihosting)         

int ppdd = 0;    

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//printf输出重新定向到串口输出
int fputc(int ch, FILE *f)
{ 	
	if(ppdd = 0)
	{
		USART_SendData(USART1,ch);  //通过串口发送数据
		//等待数据发送完毕
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);    
	}
	else
	{
		USART_SendData(USART3,ch);  //通过串口发送数据
		//等待数据发送完毕
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
		ppdd = 0;
	}
	return ch;
}

void fpd(int sum)
{
	ppdd = sum;
}