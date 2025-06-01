/*************************
�༶��������Ϣ����211
���ߣ� ����					����ԺУ������ǭ�ϿƼ�ѧԺ
�绰�� 18885484220			����:2597443135@qq.com

��ǰҳ�棺��ʪ�ȼ��  ͷ�ļ�(dht11.h)
*************************/


#ifndef __DHT11_H_
#define __DHT11_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "printf.h"
#define  TIM_SEL 1 

void Dht11_Init(void); 
void Dht11_Pin_Mode(GPIOMode_TypeDef mode);
int Dht11_Start(void);
u8 Dht11_Recv_Byte(void);
int Dht11_Date(u8 *data);

#endif