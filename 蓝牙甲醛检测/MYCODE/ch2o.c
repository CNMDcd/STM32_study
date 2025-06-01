/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺��ȩ��� ʵ���ļ�(ch2o.c)
*************************/

#include "ch2o.h"

/****
��ȩ���ģ�飺JW01
	�ź��շ��� GPIOB_Pin_10  GPIOA_Pin_11  
	ZE08-CH2O ��ͣʹ��
	SGP30 �ο� δʹ��
	JW01 ʹ��
	GY- SGP30 �ο� δʹ��

���ڣ�USART3
****/
	    
u8 flag = 0;  		// �����Ƿ�������
u8 count = 0;		// ���ݽ���λ
u8 data[9] = {0};	// ������������
u8 rxdata[9] = {0};	// �������

float ch2o_init;
int co2_init;
float tvoc_init;

void Ch2o_Init(void)
{  	 
  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	// ����ʱ��ʹ�ܣ�GPIOʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// �������Ÿ�����ӳ�䣺���� GPIO_PinAFConfig ����
	// PB10ѡ���ù���ΪUSART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	// PB11ѡ���ù���ΪUSART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// GPIO ��ʼ�����ã�Ҫ����ģʽΪ���ù���
	// ��������10��11

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10|GPIO_Pin_11;	// ����10 11
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;				// ����ģʽ
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	
	USART_InitStruct.USART_BaudRate				= 9600;					// ������
	USART_InitStruct.USART_Mode					= USART_Mode_Rx|USART_Mode_Tx;	// ȫ˫��
	USART_InitStruct.USART_Parity				= USART_Parity_No;				// ��У��λ
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;				// ֹͣλ
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;			// �ֳ�:1�ֽ�
	USART_InitStruct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;// ��Ӳ��������
	// ���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ�����
	USART_Init(USART3, &USART_InitStruct);
	
	// �����жϲ��ҳ�ʼ�� NVIC��ʹ���жϣ������Ҫ���������жϲ���Ҫ������裩��		
	NVIC_InitStructure.NVIC_IRQChannel 						= USART3_IRQn;	// �ж�ͨ����stm32f4xx.h����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;			// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x02;			// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		// ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	// ����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	// ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
	//printf("hello\r\n");
}



// ����3
void USART3_IRQHandler(void)
{ 
	//�жϱ�־λ�Ƿ���1
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		//printf("hello\r\n");
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		// ��������,ÿ��ֻ����һ���ַ�
		data[count] = USART_ReceiveData(USART3);
		 
//		printf("data[%d] = %d\r\n",count,data[count]);
//		if(data[count] == 228)
//		{
//			if(data[count - 1] == 44)
//			{
//				data[0] = 44;
//				data[1] = 228;
//				count  = 1;
//			} 
//		} 
		// 0 1 2 3 4 5 6 7 8 
		count += 1;
		
		if(  count == 9)
		{
			// ��ʾһ֡���ݽ������ 
			for(int i=0; i<count-1; i++)
			{
				// �����ݴӻ�������ŵ�����������
				rxdata[i] = data[i];
			}
			// ������ݽ��ջ�����
			memset(data, 0, sizeof(data));
			flag = 1;    // ��ʾ֡���ݽ������
			count = 0;   // ��һ֡���ݴ�g_buffer[0]��ʼ�������� 
		}
	}
}


u8 CheckSum()
{
	u8 add = 0x00;
	if(flag == 1)
	{
		
		for(int i = 0; i < 5; i++)
			add += rxdata[i];
		
		co2_init = (rxdata[6]*254 + rxdata[7]);
		tvoc_init = (rxdata[2]*254 + rxdata[3]) * 0.001;
		ch2o_init = (rxdata[4]*254 + rxdata[5]) * 0.001;
		//printf("��ȩ��%.4fmg/m3\r\n",ch2o_init);  
		//printf("CO2��%.2fmg/m3\r\n",co2_init);  
		//printf("TVOC��%.4fmg/m3\r\n",tvoc_init); 
		flag = 0;
	}
	
	if(add == data[8])
		return 0;
	else
		return 1;
}


float ch2oInit()
{
	 
	//if(CheckSum() == 1)
	//{
		//ch2o_init = (rxdata[4]*254 + rxdata[5]) * 0.001;
		return ch2o_init;
	//}
	//else
		return 0; 
}

int co2Init()
{
	 
	//if(CheckSum() == 1)
	//{
		//co2_init = (rxdata[6]*254 + rxdata[7]) * 0.001;
		return co2_init;
	//}
	//else
	//	return 0; 
}

float tvocInit()
{
	 
	//if(CheckSum() == 1)
	//{
		//tvoc_init = (rxdata[2]*254 + rxdata[3]) * 0.001;
		return tvoc_init;
	//}
	//else
	//	return 0; 
}

