#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "sys.h"

/*************************************************
*    TIM2 : PA0,PA1 to encoder port                                            
*                                                
*    TIM4 : PB6,PB7 to encoder port                                                                                         
*************************************************/

#define ENCODER_TIM_PERIOD    65535



void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(uint8_t TIMx);
void TIM4_IRQHandle(void);
void TIM2_IRQHandle(void);

#endif
