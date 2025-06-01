#ifndef __LED_H
#define __LED_H


#include "stm32f4xx.h"
#define LED0(x)		x?GPIO_SetBits(GPIOF, GPIO_Pin_9):GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define LED1(x)		x?GPIO_SetBits(GPIOF, GPIO_Pin_10):GPIO_ResetBits(GPIOF, GPIO_Pin_10)
#define LED2(x)		x?GPIO_SetBits(GPIOE, GPIO_Pin_13):GPIO_ResetBits(GPIOE, GPIO_Pin_13)
#define LED3(x)		x?GPIO_SetBits(GPIOE, GPIO_Pin_14):GPIO_ResetBits(GPIOE, GPIO_Pin_14)

#define LED0_ON		GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define LED0_OFF	GPIO_SetBits(GPIOF, GPIO_Pin_9)

#define LED1_ON		GPIO_ResetBits(GPIOF, GPIO_Pin_10)
#define LED1_OFF	GPIO_SetBits(GPIOF, GPIO_Pin_10)

#define LED2_ON		GPIO_ResetBits(GPIOE, GPIO_Pin_13)
#define LED2_OFF	GPIO_SetBits(GPIOE, GPIO_Pin_13)

#define LED3_ON		GPIO_ResetBits(GPIOE, GPIO_Pin_14)
#define LED3_OFF	GPIO_SetBits(GPIOE, GPIO_Pin_14)



void Led_Init(void);

#endif