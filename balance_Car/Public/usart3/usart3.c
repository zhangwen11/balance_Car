#include "usart3.h"

u8 Usart3_Receive;
void uart3_init(u32 bound)
{
   GPIO_InitTypeDef    GPIO_InitStruce;
	USART_InitTypeDef  USART_InitStruce;
	NVIC_InitTypeDef   NVIC_InitStruce;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruce.GPIO_Mode=GPIO_Mode_AF_PP; //Tx 
	GPIO_InitStruce.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruce.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruce);
	
	
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_IN_FLOATING;//Rx
	GPIO_InitStruce.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruce);
	
	NVIC_InitStruce.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStruce.NVIC_IRQChannelSubPriority=3; //�����ȼ�3
	NVIC_InitStruce.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruce);
	
	USART_InitStruce.USART_BaudRate=bound;
	USART_InitStruce.USART_WordLength=USART_WordLength_8b;
	USART_InitStruce.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	USART_InitStruce.USART_Parity=USART_Parity_No; //��żУ��λ
	USART_InitStruce.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruce.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	
	USART_Init(USART3,&USART_InitStruce);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); //�������ڽ����ж�
	USART_Cmd(USART3,ENABLE);

}

/********************************************
����3�����ж�
��ڲ�������
����ֵ����
*********************************************/

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3,USART_IT_RXNE) !=RESET)
  {
     static u8 uart_receive;//����������ر���
	  uart_receive=USART3->DR;//uart_receive=USART_ReceiveData(USART3);
	  
	  switch(uart_receive)//SBUF
	  {
		  case 'A':
			          Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0; ////////////////////ǰ��
		              break;
		  case 'B':
                      Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0; ////////////////////����
                      break;
          case 'C':  
                      Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0; ////////////////////��ת
                      break;
          case 'D':
                      Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1; ////////////////////��ת
                      break;
//		  case 'S':  
//			         Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
//                      break;
		  default:
			         Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;
                      break;
	  }
  
  }
  
}
