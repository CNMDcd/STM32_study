#include "printf.h"

#pragma import(__use_no_semihosting)         

int ppdd = 0;    

//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//printf������¶��򵽴������
int fputc(int ch, FILE *f)
{ 	
	if(ppdd = 0)
	{
		USART_SendData(USART1,ch);  //ͨ�����ڷ�������
		//�ȴ����ݷ������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);    
	}
	else
	{
		USART_SendData(USART3,ch);  //ͨ�����ڷ�������
		//�ȴ����ݷ������
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
		ppdd = 0;
	}
	return ch;
}

void fpd(int sum)
{
	ppdd = sum;
}