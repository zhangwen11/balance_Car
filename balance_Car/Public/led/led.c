#include "led.h"


//led≥ı ºªØ
void led_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStuce;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStuce.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStuce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStuce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStuce);

}


void Led_Flash(uint16_t time)
{
    static  int temp;

    if(0 == time) led = 0;
    else      if(++temp == time)  led = ~led, temp = 0;

}
