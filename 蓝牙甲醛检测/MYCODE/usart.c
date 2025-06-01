#include "usart.h"


/*********************************
����˵����

PA9  ---- USART1_TX(���Ͷ�)
PA10 ---- USART1_RX(���ն�)

**********************************/

void Usart1_Init(int my_BaudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;	
	
	//����ʱ��ʹ�ܣ�GPIOA ʱ��ʹ�ܡ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//�������Ÿ�����ӳ�䣺���� GPIO_PinAFConfig ������
	//PA9ѡ���ù���ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);	
	//PA10ѡ���ù���ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);	

	
	//GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù��ܡ�
	//��������9
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;   	//����9 10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;  	//����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	
	
	USART_InitStruct.USART_BaudRate	= my_BaudRate;		//������
	USART_InitStruct.USART_Mode		= USART_Mode_Rx|USART_Mode_Tx; //ȫ˫��	
	USART_InitStruct.USART_Parity	= USART_Parity_No; //��У��λ
	USART_InitStruct.USART_StopBits	= USART_StopBits_1; //ֹͣλ
	USART_InitStruct.USART_WordLength= USART_WordLength_8b;	//�ֳ���1�ֽ�
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	//���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ�����
	USART_Init(USART1, &USART_InitStruct);
	
	
	
	//�����жϲ��ҳ�ʼ�� NVIC��ʹ���жϣ������Ҫ���������жϲ���Ҫ������裩��
	NVIC_InitStructure.NVIC_IRQChannel 						= USART1_IRQn;   //�ж�ͨ����stm32f4xx.h����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x01;	//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;  //ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	
	//����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    
	//ʹ�ܴ��ڡ�
	USART_Cmd(USART1, ENABLE);

}