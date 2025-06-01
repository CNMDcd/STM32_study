/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：定时器延时  头文件(delay.h)
*************************/


#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f4xx.h"

void Delay_Init(void);
void delay_us(int nus);
void delay_ms(int nms);	
void delay_s(int ns);
	
#endif

