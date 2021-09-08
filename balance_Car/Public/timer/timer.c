#include "timer.h"
#include "led.h"

/***********************************************
定时器中断初始化

************************************************/


void Timer1_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef        TIM_TimBaseStruce;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    NVIC_InitTypeDef            NVIC_InitStruce;

    TIM_TimBaseStruce.TIM_Period = arr;
    TIM_TimBaseStruce.TIM_Prescaler = psc;
    TIM_TimBaseStruce.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimBaseStruce.TIM_ClockDivision = 0;
    TIM_TimBaseStruce.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimBaseStruce);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update); //清除中断标志位
    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

    NVIC_InitStruce.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruce.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruce);
	
    TIM_Cmd(TIM1,ENABLE);



}
