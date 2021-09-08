#ifndef __USART3_H
#define __USART3_H

#include "sys.h"

extern u8 Usart3_Receive;
void uart3_init(u32 bound);
void USART3_IRQHandle(void);
#endif
