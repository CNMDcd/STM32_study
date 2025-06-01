#include "stm32f4xx.h"
#include "stdio.h"
#include "led.h"
#include "delay.h"
#include "pwm.h"
#include "sr04.h"
#include "printf.h"
#include "usart.h"
#include "bluetooth.h"
#include "OLED_I2C.h"
#include "SG90.h"
#include "iic.h" 

#include "FreeRTOS.h"
#include "task.h"


/*
��Ŀ�����״�ʦ

PWM  ռ�ձ�
1. PC6  TIM3_CH3 -- DCMI_D0			// ��
2. PC7  TIM3_CH4 -- DCMI_D1
3. PC8  TIM3_CH3 -- DCMI_D2
4. PC9  TIM3_CH4 -- DCMI_D3

5. PD14 TIM4_CH3 -- FSMC_D0			// ��
6. PD15 TIM4_CH4 -- FSMC_D1	

7. PA2 	TIM5_CH3 -- USART2
8. PA3  TIM5_CH4 -- USART2

����  USART3
	PB10  	USART3_TX
	PB11  	USART3_RX

���������
	PE5 ��Ϊ��ͨ���TRIG�������źţ�	DCMI_D6
	PE6 ��Ϊ��ͨ����ECHO�������źţ�	DCMI_D7


OLED
	PB15  	LCD_BL      SCK
	PD10  	FSMC_D15    SDA

���SG90
	PB7		DCMI_VSYNC	��ѹ���ƣ���ʹ��PWM����

����ʶ��ASR01
	PA2 	USART1
	PA3  	USART1

*/
 
 




/***  ����(bluetooth)  ***/
u8 g_flag = 0;  			// �ж��Ƿ����������
u8 g_buffer[32] = {0};    	// ��������
u8 g_rxbuffer[32] = {0};  	// ��ȡ����
u8 rx_i;					// �ж�����
u8 g_count = 0; 			// �������ݳ���

/*** ����״̬��ʶ 	***/
u8 catPd = 0;				// 0 ֹͣ�� 1 ��ʻ
u8 catPdto = 0;				// 0 �ֶ�ģʽ�� 1 �Զ�����ģʽ
u8 catPdtou = 0;			// 0 ��ǰ, 		1 ���
int catSum = 600;			// ��ʻ�ٶ�
int catSumMax = 40;			// �趨��ȫ����
int catSumto = 0;			// ʵʱ���
int catSumtoPd = 0;			// ����Ƿ����ϰ���
int catPdLR = 0;			// ת�� 0 ֱ�� 1 �� 2 ��

/*** ���SG90     ***/
int sg90Up = 0;


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
			
			g_flag = 2; 
			 
			g_count = 0;
	
		} 
	}	 
}

void USART3_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
		
		g_buffer[g_count++] = USART_ReceiveData(USART3);   
		if(g_buffer[g_count-1] == ':')
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

/* ����1  ��ʼ�� */ 
static TaskHandle_t app_init_handle = NULL;
static void app_init(void* pvParameters);  


/* ����2  OLED */
static TaskHandle_t app_OLED_handle = NULL;
static void app_OLED(void* pvParameters);  

/* ����2  LED */
static TaskHandle_t app_LED_handle = NULL;
static void app_LED(void* pvParameters);  

/* ����3  BLUE*/ 
static TaskHandle_t app_BLUE_handle = NULL;
static void app_BLUE(void* pvParameters);  

/* ����3  BLUE*/ 
static TaskHandle_t app_SG90_handle = NULL;
static void app_SG90(void* pvParameters);  

/* ����5  LeftLED*/ 
static TaskHandle_t app_LeftLED_handle = NULL;
static void app_LeftLED(void* pvParameters);  

/* ����6  RightLED*/ 
static TaskHandle_t app_RightLED_handle = NULL;
static void app_RightLED(void* pvParameters);  

int main(void)
{
 

	/* ����app_blue���� */
	xTaskCreate((TaskFunction_t )app_init,  		/* ������ں��� */
			  (const char*    )"app_init",			/* �������� */
			  (uint16_t       )64,  				/* ����ջ��С */
			  (void*          )NULL,				/* ������ں������� */
			  (UBaseType_t    )1, 					/* ��������ȼ� */
			  (TaskHandle_t*  )&app_init_handle);	/* ������ƿ�ָ�� */ 

 
//	/* ����������� */
	vTaskStartScheduler(); 
 
}

static void app_init(void* pvParameters)
{ 	
	Led_Init();
 	Pwm_one_Init();
 	Usart1_Init(115200);
 	Bluetooth_Init();
 	Sr04_Init();
 	I2C_Configuration();
 	sg90_Init();
	sg90_up(0);
	Iic_Init();
 	//sg90_up(90); 

 	OLED_Init();
 	OLED_CLS(); 

	//TIM_SetCompare3(TIM5,500);		// PA USART2
	//TIM_SetCompare4(TIM5,0);
	//�����û�����1
	xTaskCreate(app_LED, "LED", 64, NULL, 3, &app_LED_handle);
	xTaskCreate(app_OLED, "OLED", 64, NULL, 3, &app_OLED_handle);
	
	xTaskCreate(app_SG90, "SG90", 64, NULL, 3, &app_SG90_handle);
	xTaskCreate(app_BLUE, "BLUE",128, NULL,3, &app_BLUE_handle);	
	
	//xTaskCreate(app_LeftLED, "LeftLED", 128, NULL, 3, &app_LeftLED_handle);
	//xTaskCreate(app_RightLED, "RightLED", 128, NULL, 3, &app_RightLED_handle);
	
	
	for(;;)
	{
	 

		GPIO_ToggleBits(GPIOF, GPIO_Pin_10); 
		printf("app_init\r\n");
		delay_s(1);

	}
}  

// ����ָ���
static void app_BLUE(void* pvParameters)
{
	printf("app_BLUE\r\n");
	for(;;)
	{
		
		if(g_flag == 1 || g_flag == 2)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13); 
			printf("app_BLUE\r\n");
			if(strncmp(g_rxbuffer,"tou",3) == 0)
			{
				/*
				���ҡ�ڿ���
				*/
				if(catPdto == 0)
				{
					char *p = strtok(g_rxbuffer,"-");
					printf("����1��%s\r\n",p);
					if(p != NULL)
						p = strtok(NULL,"-");
					printf("����2��%s\r\n",p);
					int sum = atoi(p);
					printf("����SG90UP��%d\r\n",sum);
					sg90Up = sum;
					
				}
			}
			if(strncmp(g_rxbuffer,"SYSTEM",6) == 0)
			{ 
				/*
				��ȫ�������
				*/
				char *p = strtok(g_rxbuffer,"/");
				printf("����1��%s\r\n",p);
				if(p != NULL)
						p = strtok(NULL,"-");
				printf("����2��%s\r\n",p);
				int sum = atoi(p);
				printf("����3��%d\r\n",sum); 
				catSumMax = sum;
				char tmp[8];
				sprintf(tmp,"%d",sum);
				At24c02_Write_Page(0x00, tmp, 8);
			 
			}
/******************************************************
u8 catPd = 0;				// 0 ֹͣ�� 1 ��ʻ
u8 catPdto = 0;				// 0 �ֶ�ģʽ�� 1 �Զ�����ģʽ
u8 catPdtou = 0;			// 0 ��ǰ, 		1 ���
u16 catSum = 500;			// ��ʻ�ٶ�
u16 catSumMax = 40;			// �趨��ȫ����
int catSumto = 0;			// ʵʱ���

	TIM_SetCompare3(TIM5,0);		// PA USART2	�Һ�
	TIM_SetCompare4(TIM5,0);	
	TIM_SetCompare1(TIM3,0);		// PC DCMI		��ǰ
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);		// PC DCMI		���
	TIM_SetCompare4(TIM3,0);
	TIM_SetCompare3(TIM4,0);		// PD FSMC		��ǰ
	TIM_SetCompare4(TIM4,0);		
******************************************************/
			if(strcmp(g_rxbuffer,"STA/STO") == 0)
			{
				/*
				��������
				*/
				printf("����4��%s\r\n",g_rxbuffer);
				if(catPd == 0)
				{
					catPd = 1;
					catPdtou = 0;
					printf("����5��%s\r\n",g_rxbuffer);
					cat_start(catSum);
				}
				else
				{
					catPd = 0;
					cat_stop();
 				}
				
			} 
			if(strcmp(g_rxbuffer,"VANCE") == 0)
			{	// ǰ��
				if(catPd == 1 || catPdto == 0)
				{
					if(catPdtou == 0)
					{
						catSum += 99;
					}
					else
					{
						catPdtou = 0;
						catSum = 600;
					} 
					cat_Advance(catSum);
				}
			}
			if(strcmp(g_rxbuffer,"BACK") == 0)
			{	// ����
				if(catPd == 1 || catPdto == 0)
				{
					if(catPdtou == 1)
					{
						catSum += 99;
					}
					else
					{
						catPdtou = 1;
						catSum = 600;
					}
					cat_Backoff(catSum);
				}
			}
			if(strcmp(g_rxbuffer,"LEFT") == 0)
			{	// ��ת
				if(catPd == 1 || catPdto == 0)
				{
					if(catPdtou == 0)
					{
						if(catPd == 1 && catPdto == 0)	// ��ǰ��ת
						{
							cat_AdvanceLeft(catSum);
						}
					}
					else
					{
						if(catPd == 1 && catPdto == 0)	// �����ת
						{
							cat_BackoffLeft(catSum);
						}
					} 
					catPdLR = 1;
				}
			}
			if(strcmp(g_rxbuffer,"RIGHT") == 0)
			{	// ��ת
				if(catPd == 1 || catPdto == 0)
				{
					if(catPdtou == 0)
					{
						if(catPd == 1 && catPdto == 0)	// ��ǰ��ת
						{
							cat_AdvanceRight(catSum);
						}
					}
					else
					{
						if(catPd == 1 && catPdto == 0)
						{
							cat_BackoffRight(catSum);
						}
					} 
					catPdLR = 2;
				}
			}
			if(strcmp(g_rxbuffer,"LEFTSS") == 0)
			{	// ��			
				if(catPdto == 0)
				{
					cat_Leftss();
				}
			}
			if(strcmp(g_rxbuffer,"RIGHTSS") == 0)
			{	// �һ�
				if(catPdto == 0)
				{					
					cat_Rightss();
				}
			}
			if(strcmp(g_rxbuffer,"MY/AI") == 0)
			{	// �л���ʻģʽ
				if(catPdto == 0)
				{
					catPdto = 1;
					catPdtou = 0;
					cat_start(600);
				}
				else
				{
					catPdto = 0;
				}
			}
		


			printf("g_rx:%s\r\n",g_rxbuffer);
			g_flag = 0;
			memset(g_rxbuffer, 0, sizeof(g_rxbuffer));
			delay_ms(1);
		} 
		if(catPdto == 1 && catPdto == 1)
		{	// �Զ�����ģʽ
			if(catSumtoPd == 1)
			{
	printf("catSumto < catSumMax\r\n");
				catPdtou = 1;
				catSum = 600;
				cat_Backoff(600);
				delay_s(1);
				catPdtou = 0;
				//cat_Advance(600);
				cat_AdvanceLeft(catSum);
				delay_s(1);
				cat_Advance(400);
				//catPdLR = 1;
				catSumtoPd = 0;
			}
			else
			{
				cat_start(400);
			}
		}
		
	}
}  


// OLED��ʾ
static void app_OLED(void* pvParameters)
{

	printf("app_OLED\r\n");	
	int cont = 0;
	int sont = 0;
	
	for(;;)
	{
		OLED_Fill(0x00);
		//delay_ms(10);
		catSumto = Get_Sr04_Value();			// ʵʱ���
		delay_ms(30);
		char add[20];
		if(catSumto <= 0)
		{
			catSumto = 0;
			sprintf(add,":errorCM"); 
		}
		else
		{
			sprintf(add,":%dCM",catSumto); 
		}
		 

		OLED_ShowCN(0,0,8);
		OLED_ShowCN(16,0,9);
		OLED_ShowCN(32,0,10);
		OLED_ShowCN(48,0,11);
		OLED_ShowStr(64,0,add,2);

		char read_buff[24];
		At24c02_Read_Data(0x00, read_buff, 8);
		OLED_ShowStr(0,4,read_buff,2);
		catSumMax = atoi(read_buff);
		
		if(catSumto < catSumMax)
		{
			catSumtoPd = 1;
		}

		char add1[20];
		sprintf(add1,":%dCM",catSumMax); 
		OLED_ShowCN(0,2,12);
		OLED_ShowCN(16,2,13);
		OLED_ShowCN(32,2,11);
		OLED_ShowCN(48,2,14);
		OLED_ShowStr(64,2,add1,2);

		
		
		fpd(2);
		printf("ʵʱ���룺%dCM\r\n",catSumto);
		printf("��ȫ����:%s(%d)\r\n",read_buff,catSumtoPd);
		delay_s(1);

	}
}  

/* LED ���� */
static void app_LED(void* pvParameters)
{
 
	printf("app_LED\r\n");	
	int cont = 0; 
	int sont = 0;
	int input = 0;
	for(;;)
	{ 

//		if(catPdLR == 0 && input == 0)
//		{
//			input = 1;

//			vTaskSuspend(&app_LeftLED_handle);
//			GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
//			vTaskSuspend(&app_RightLED_handle);
//		}
//		
//		if(catPdLR == 1)
//		{
//			vTaskResume(app_LeftLED_handle);
//			cont ++;
//		}
//		if(catPdLR == 2)
//		{
//			vTaskResume(app_RightLED_handle);
//			sont ++;
//		}
//		if(cont > 5 || sont > 5)
//		{
//			input = 0;
//			catPdLR = 0;
//			cont = 0;
//			sont = 0;
//		}

		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);		
		delay_s(1);		
 
	}
}  


/* SG90 ���� */
static void app_SG90(void* pvParameters)
{

	printf("app_SG90\r\n");
	for(;;)
	{ 
		//printf("app_SG-(%d)\r\n",sg90Up);
		sg90_up(sg90Up);
		delay_us(10);
	}
}  


/* LED ���� */
static void app_LeftLED(void* pvParameters)
{
 
	for(;;)
	{ 

		if(catPdLR == 1)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
		}
		delay_ms(500);		
	}
}  

/* LED ���� */
static void app_RightLED(void* pvParameters)
{
 
	for(;;)
	{ 
		GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		delay_ms(500);		
	} 
}  



