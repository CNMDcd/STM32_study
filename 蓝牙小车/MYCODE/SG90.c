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
	/* GPIOBʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					//��8������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//����Ϊ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//����Ҫ��������
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
 
