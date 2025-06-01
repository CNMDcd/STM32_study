#include "sr04.h"


/*********************************
����˵����

 TRIG�������źţ�  
 ECHO�������źţ�

PE5 ��Ϊ��ͨ���	DCMI_D6
PE6 ��Ϊ��ͨ����	DCMI_D7

**********************************/
void Sr04_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
    
    //1���ܶ�ʱ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	//ʹ��GEPOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5; //����2
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT; 	//���ģʽ
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//����ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz; //����ٶ�
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6; 		//����3
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN; 	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP; 	//����
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	

    
    TIM_TimeBaseInitStruct.TIM_Prescaler    = 84-1;                  // 84��Ƶ 84MHZ/84 = 1MHZ
    TIM_TimeBaseInitStruct.TIM_Period    	= 50000-1;                        
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;    // ���ϼ���
    TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;            // ��Ƶ����
    //2����ʼ����ʱ��������ARR,PSC
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
            
    //5,��ʹ�ܶ�ʱ��
    TIM_Cmd(TIM2, DISABLE);

}

//����-1: ����ʧ��
//����-2����ʾ����ʧ��
//��������ֵ����ʾ�����ɹ�

int Get_Sr04_Value(void)
{
	int dis, temp, t = 0;
	
	//�����ź�
	//PE5����͵�ƽ
	PEout(5) = 0;
	delay_us(8);
	//PE5����ߵ�ƽ
	PEout(5) = 1;
	delay_us(20);
	//PE5����͵�ƽ
	PEout(2) = 0;
	
	
	//���ö�ʱ����CNTΪ0  
	TIM2->CNT = 0;
	
	
	t = 0;
	//PE6�ȴ��ߵ�ƽ�������ο������ɿ����� 
	while( PEin(6) == 0)
	{
		t++;
		delay_us(10);
		
		if(t >= 500)  //5000usδ��⵽�ߵ�ƽ�����˳�
		{
			return -1;
		}
	}

	//ʹ�ܶ�ʱ����ʼ����
    TIM_Cmd(TIM2, ENABLE);
	
	
	t = 0;
	//PA3�ȴ��͵�ƽ������while( ����ƽ == 1);
	while( PEin(6) == 1)
	{
		t++;
		delay_us(10);
		
		
		if(t >= 2900)  //�ȴ�29msδ�е͵�ƽ�������򷵻�
		{
			return -2;
		}		
		
	}

	//��ȡ��ʱ��CNTֵ���Ӷ��õ��ߵ�ƽ����ʱ��    
	temp = TIM2->CNT;

	//�رն�ʱ��
	TIM_Cmd(TIM2, DISABLE);

	//ͨ����ʽ�������������������	temp�Ǹߵ�ƽ����ʱ��
	dis = temp/58;
	
	return dis;
}
	
