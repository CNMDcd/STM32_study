/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺������Ϣ��ʾ ʵ���ļ�(printf.c)
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




