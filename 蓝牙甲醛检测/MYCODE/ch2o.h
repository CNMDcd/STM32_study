/*************************
班级：电子信息工程211
作者： 文龙					所属院校：贵州黔南科技学院
电话： 18885484220			邮箱:2597443135@qq.com

当前页面：甲醛检测 头文件(ch2o.h)
*************************/

#ifndef __CH2O_H_
#define __CH2O_H_

#include "stm32f4xx.h"
#include "printf.h"

void Ch2o_Init(void);
u8 CheckSum();
float ch2oInit();
int co2Init();
float tvocInit();

#endif
