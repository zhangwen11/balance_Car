#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "sys.h"

#define PWMA    TIM3->CCR4 //PB1
#define AIN2    PBout(15)
#define AIN1    PBout(14)
#define BIN1    PBout(12)
#define BIN2    PBout(13)
#define PWMB    TIM3->CCR3 //PB0


void MiniBalance_Motor_Init(void);
void MiniBalance_PWM_Init(u16 arr, u16 psc);
#endif
