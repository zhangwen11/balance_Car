#ifndef __LED_H_
#define __LED_H_

#include "sys.h"

void led_init(void);
void Led_Flash(uint16_t time);

#define led PCout(13)  //PC13

#endif 
