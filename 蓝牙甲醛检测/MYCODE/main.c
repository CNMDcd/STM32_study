/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺������(main.c)
*************************/

#include "stm32f4xx.h"
#include "pwm.h"
#include "bluetooth.h"
#include "printf.h"
#include "hcsr04.h"
#include "delay.h"
#include "led.h"
#include "dht11.h"
#include "tim3.h"
#include "ch2o.h"
#include "OLED_I2C.h"
#include "iic.h"
#include "beep.h"

/*************************
// OLED ��ʾ��		���
	GPIOB_Pin_3  SCK
	GPIOB_Pin_4	 SDA
	
// ��ȩ���			���
	���� USART3	 B9600
	GPIOB_Pin_10 ��������
	GPIOB_Pin_11 ���շ�������

	��ʱ����TIM2

// ��ʪ�ȼ��		���
	GPIOE_Pin_1

	��ʱ����TIM3

// ��Ʋ���			���
	GPIOA_Pin_6 
	
// ���������		���
	GPIOB_Pin_8 ��������
	GPIOB_Pin_9 ���շ�������

	��ʱ����TIM4


// ����				���
	���� USART1	B9600
		GPIOA_Pin_9 GPIOA_Pin_10;

// �����������  	���
	�ķ�� ����������������Ч 
		//PA0 PA1
		TIM_SetCompare1(TIM5, 0);
		TIM_SetCompare2(TIM5, 0); 
	���(����) ��������������������з�������ת
		//PA2 PA3
		TIM_SetCompare3(TIM5, 0); 
		TIM_SetCompare4(TIM5, 0); 

	��ʱ����TIM5
		
*************************/


unsigned char i;
extern const unsigned char BMP1[];
	
u8 g_data = 0;					// �������������


int time = 0;

float ch2o = 1.0;
int co2 = 1;
float tvoc = 1.0;
u8 Dht11_Tmp[5] = {0}; 
int ret = 0;;

/***  ����(bluetooth)  ***/
u8 g_flag = 0;  			// �ж��Ƿ����������
u8 g_buffer[32] = {0};    	// ��������
u8 g_rxbuffer[32] = {0};  	// ��ȡ����
u8 rx_i;					// �ж�����
u8 g_count = 0; 			// �������ݳ���

void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
		
		g_buffer[g_count++] = USART_ReceiveData(USART1); 
		
		//printf("g_count:%d\r\n",g_count);
		if(g_buffer[g_count - 1] == ':')
		{ 
			for(rx_i=0; rx_i < g_count - 1; rx_i++)
			{ 
				g_rxbuffer[rx_i] = g_buffer[rx_i];
			}
			 
			memset(g_buffer, 0, sizeof(g_buffer));
			
			g_flag = 1; 
			 
			g_count = 0;
	
		} 
	}	 
}

void  TIM2_IRQHandler(void)
{
	//�жϱ�־λ�Ƿ���1
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		time++;
		if(time == 10)
		{
			ch2o = ch2oInit();
			co2 = co2Init();
			tvoc = tvocInit();
 
			time = 0;
		}
		
		//OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ 
		//GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	

}
 

int main(void)
{
	int pd_Mode = 1;
	Pwm_Init();
	Bluetooth_Init();
	Hcsr04_Init();
	Led_Init();
	Tim3_Init();
	Dht11_Init();  
	Ch2o_Init();
	beep_Init();
	
	
	Iic_Init();
	I2C_Configuration();
	OLED_Init(); 

	CheckSum();
	delay_ms(10);
	ch2o = ch2oInit();
	co2 = co2Init();
	tvoc = tvocInit();
 
	while(1)
	{  
		
		delay_ms(50);
		CheckSum();
		delay_ms(50);
		OLED_Fill(0x00);//ȫ����
		delay_ms(10);  
		OLED_ShowStr(2, 0, "ch2o:", 1); 
		char ch2o2[11] = {0};
		sprintf(ch2o2,"%.4f",ch2o);
		OLED_ShowStr(32, 0, ch2o2, 1); 
		OLED_ShowStr(92, 0, "mg/m3", 1); 

		OLED_ShowStr(2, 1, "tvoc:", 1); 
		char tvoc2[11] = {0};
		sprintf(tvoc2,"%.4f",tvoc);
		OLED_ShowStr(32, 1, tvoc2, 1); 
		OLED_ShowStr(92, 1, "mg/m3", 1); 

		OLED_ShowStr(2, 2, "co2:", 1); 
		char co22[11] = {0};
		sprintf(co22,"%d",co2);
		OLED_ShowStr(32, 2, co22, 1); 
		OLED_ShowStr(92, 2, "PPM", 1); 
 
		delay_ms(50);
		ret = Dht11_Date(Dht11_Tmp);
		delay_ms(50);

		g_data = Get_Hcsr04_Value();
		if(ret == 1)
		{
 
			char cs[20] = {0};
			char cs1[20] = {0};
			sprintf(cs,"WenDu:%d.%d oC",Dht11_Tmp[2],Dht11_Tmp[3]);
			sprintf(cs1,"ShiDu:%d.%d",Dht11_Tmp[0],Dht11_Tmp[1]);
			OLED_ShowStr(2, 5, cs, 1); 
			OLED_ShowStr(2, 6, cs1, 1); 
		} 
		else
			OLED_ShowStr(2, 4, "error", 1); 
		
		

		if(pd_Mode == 1)
		{
			printf("��ȩ��%.4fmg/m3\r\n",ch2o);  
			printf("TVOC��%.4fmg/m3\r\n",tvoc);  
			printf("CO2��%.dPPM\r\n",co2);  
			
			printf("�¶�:%d.%d��,ʪ��:%d.%d\r\n", Dht11_Tmp[2],Dht11_Tmp[3], Dht11_Tmp[0], Dht11_Tmp[1]);
			printf("Distance: %d CM\r\n", g_data);
			if(ch2o >= 0.1)
			{
				printf("ch2oŨ�ȳ���\r\n");
				GPIO_ResetBits(GPIOE,GPIO_Pin_1);
				TIM_SetCompare1(TIM5, 999); 
				TIM_SetCompare3(TIM5, 999);  
			}   
			else
			{
				GPIO_SetBits(GPIOE,GPIO_Pin_1);
				TIM_SetCompare1(TIM5, 0); 
				TIM_SetCompare3(TIM5, 0);  
			}
				
		}

		if(g_flag == 1)
		{ 
			if(strcmp(g_rxbuffer, "start") == 0)
			{	// �ֶ����� �ķ���ͷ���
				TIM_SetCompare1(TIM5, 999); 
				TIM_SetCompare3(TIM5, 999);  
			}
			if(strcmp(g_rxbuffer, "stop") == 0)
			{
				// �ֶ�ֹͣ �ķ���ͷ���
				TIM_SetCompare1(TIM5, 0); 
				TIM_SetCompare3(TIM5, 0);  
			}
			if(strcmp(g_rxbuffer, "hcsr") == 0)
			{
				// �ֶ����� ���β��
				//g_data = Get_Hcsr04_Value();
				delay_ms(50);
				if(g_data != 0)
				{
					printf("Distance: %d CM\r\n", g_data); 
				}
				else
				{
					printf("Distance��ERROR\r\n"); 
				}	 
			}// 
			if(strcmp(g_rxbuffer, "ledo") == 0)
			{
				// �ֶ���֤�����Ƿ��������� PLL_M
				//printf("hello");
				delay_ms(20);
				GPIO_ToggleBits(GPIOA,GPIO_Pin_6); 
			}
			if(strcmp(g_rxbuffer, "dht11") == 0)
			{
				// �ֶ���ȡ��ʪ��
				
				if(ret == 1)//*********************
				{
					printf("�¶�:%d.%d��,ʪ��:%d.%d\r\n", Dht11_Tmp[2],Dht11_Tmp[3], Dht11_Tmp[0], Dht11_Tmp[1]);
				} 
				else
				{ 
					printf("��ʪ�Ȼ�ȡ:ʧ��\r\n");
				}   
			}
			if(strcmp(g_rxbuffer, "ch2o") == 0)
			{ 
				printf("��ȩ��%.4fmg/m3\r\n",ch2o);  
				printf("TVOC��%.4fmg/m3\r\n",tvoc);  
				printf("CO2��%.dPPM\r\n",co2);  
				 
			}
			if(strcmp(g_rxbuffer, "beep") == 0)
			{ 
				GPIO_ToggleBits(GPIOE,GPIO_Pin_1);
				u8 bee = GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_1);
				printf("gpioe_pin_1:%d\r\n",bee);
			}
			if(strcmp(g_rxbuffer, "zidong") == 0)
			{
				pd_Mode = 1;
			}
			if(strcmp(g_rxbuffer,"shoudong") == 0)
			{
				pd_Mode = 11;
			} 
			g_flag = 0;
			memset(g_rxbuffer,0,sizeof(g_rxbuffer));
		} 
		delay_s(1); 
 

	}
    return 0;
 }
