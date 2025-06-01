#include "led.h"


/*********************************
����˵����

LED0������PF9
PF9����͵�ƽ��0����������PF9����ߵ�ƽ��1��������

LED0--PF9
LED1--PF10
LED2--PE13
LED3--PE14


**********************************/


void Led_Init(void)
{
	
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//ʹ��GEPOE GPIOFʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9|GPIO_Pin_10; //����9 10
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //����
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13|GPIO_Pin_14; //����13 14
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//�����еĵ�
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);   //������1
	GPIO_SetBits(GPIOF, GPIO_Pin_10);   //������1
	GPIO_SetBits(GPIOE, GPIO_Pin_13);   //������1
	GPIO_SetBits(GPIOE, GPIO_Pin_14);   //������1

}