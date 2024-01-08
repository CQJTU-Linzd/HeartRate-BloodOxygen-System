#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void); // PWM初始化
void PWM_SetCompare1(uint16_t Compare); // 改变 PWM 波形的占空比
void PWM_On();
void PWM_Off();
#endif
