#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx.h"


void Pwm_one_Init(void);
void cat_start(int catSum);
void cat_stop(void);
void cat_Advance(int catSum);
void cat_Backoff(int catSum);

void cat_AdvanceLeft(int catSum);
void cat_BackoffLeft(int catSum);

void cat_AdvanceRight(int catSum);
void cat_BackoffRight(int catSum);

void cat_Leftss(void);
void cat_Rightss(void);

#endif