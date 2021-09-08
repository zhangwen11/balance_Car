#ifndef __CONTROL_H_
#define __CONTROL_H_
#include "sys.h"

#define PI 3.14159265
extern int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
int TIM1_UP_IRQHandler(void);
int balance(float Angle, float Gyro);
int velocity(int encoder_left, int encoder_right);
int turn(int encoder_left, int encoder_right, float gyro);
void  Set_Pwm(int moto1, int moto2);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle);
void Get_Angle(void);
int myabs(int a);


#endif
