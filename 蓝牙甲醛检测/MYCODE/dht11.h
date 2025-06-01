/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：温湿度检测  头文件(dht11.h)
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