#include "motor.h"


//电机驱动输出IO初始化
void MiniBalance_Motor_Init(void)
{
    GPIO_InitTypeDef       GPIO_InitStruce;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruce.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //PB12,PB13,PB14,PB15
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruce);

}

//电机驱动PWM输出IO初始化

void MiniBalance_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef          GPIO_InitStruce;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruce;
    TIM_OCInitTypeDef         TIM_OCInitStruce;
    MiniBalance_Motor_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);

    GPIO_InitStruce.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //TIM3 CH3,CH4为PWM
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruce);

    TIM_TimeBaseStruce.TIM_Period = arr;
    TIM_TimeBaseStruce.TIM_Prescaler = psc;
    TIM_TimeBaseStruce.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruce.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruce);

    TIM_OCInitStruce.TIM_Pulse = 0;
    TIM_OCInitStruce.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruce.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruce.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStruce);
    TIM_OC4Init(TIM3, &TIM_OCInitStruce);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);

    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);

}
