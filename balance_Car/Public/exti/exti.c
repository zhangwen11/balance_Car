#include "exti.h"


void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef  EXTI_InitStruce;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef      GPIO_InitStruce;
	
	GPIO_InitStruce.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruce.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruce.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruce);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
	
    EXTI_InitStruce.EXTI_Line=EXTI_Line3;
	EXTI_InitStruce.EXTI_LineCmd=ENABLE;
	EXTI_InitStruce.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruce.EXTI_Trigger=EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruce);
}
