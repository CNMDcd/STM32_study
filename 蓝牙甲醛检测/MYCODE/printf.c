/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：调试信息显示 实现文件(printf.c)
*************************/


#include "printf.h"



#pragma import(__use_no_semihosting)     

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;         
int _sys_exit(int x) 
{ 
	x = x; 
}  

int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);    
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  
    
	return ch;
}




