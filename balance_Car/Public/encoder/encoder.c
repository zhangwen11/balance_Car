#include "encoder.h"


/*@brief :Set TIM2 to encoder mode
* @param :no
* @retval:no
**/
void Encoder_Init_TIM2(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStruce;
    GPIO_InitTypeDef            GPIO_InitStruce;
    TIM_ICInitTypeDef           TIM_ICInitStruce;

    GPIO_InitStruce.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //Set AP0,AP1 to encoder port
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruce);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStruce);
    TIM_TimeBaseStruce.TIM_Prescaler = 0x0;
    TIM_TimeBaseStruce.TIM_Period = ENCODER_TIM_PERIOD; //Set automatic counter reloading value
    TIM_TimeBaseStruce.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruce);
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruce);
    TIM_ICInitStruce.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStruce);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);

}

/*@brief :Set TIM4 to encoder mode
* @param :no
* @retval:no
**/
void Encoder_Init_TIM4(void)
{
    TIM_TimeBaseInitTypeDef       TIM_TimeBaseStruce;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef             GPIO_InitStruce;
    TIM_ICInitTypeDef            TIM_ICInitStruce;

    GPIO_InitStruce.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStruce);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStruce);
    TIM_TimeBaseStruce.TIM_Period = ENCODER_TIM_PERIOD;
    TIM_TimeBaseStruce.TIM_Prescaler = 0x0;
    TIM_TimeBaseStruce.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruce.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruce);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruce);
    TIM_ICInitStruce.TIM_ICFilter = 10;
    TIM_ICInit(TIM4, &TIM_ICInitStruce);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);


    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, ENABLE);

}

/*
*  @brief： Encoder count read per unit time
*  @param:  TIMx
*  @retval: speed
**/
int Read_Encoder(uint8_t TIMx)
{
    int Encoder_TIM;

    switch(TIMx)
    {
        case 2:
            Encoder_TIM = (short)TIM2 -> CNT;  //读取CNT的计数值给变量，再清除CNT的计数值，每次读出来的计数值就是速度
            TIM2 ->CNT=0;
            break;

        case 4:
            Encoder_TIM = (short)TIM2 -> CNT;
            TIM2 ->CNT=0;
            break;

        default:
            Encoder_TIM=0;
    }

    return Encoder_TIM;
}

/*@brief : IRQHandle function of TIM2
* @param ：no
* @retval：no
**/
void TIM4_IRQHandler(void)
{
    if(TIM4->SR&0X0001) //Overflow interrupt
    {
    }

    TIM4->SR&=~(1<<0); //Clear interrupt flag bit
}
/*@brief ：IRQHandle function of TIM2
* @param ：no
* @retval：no
**/

void TIM2_IRQHandler(void)
{
    if(TIM2->SR&0X0001)
    {
    }

    TIM2->SR&=~(1<<0);

}
