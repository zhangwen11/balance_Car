#include "key.h"

/*@brief:������ʼ��
*@param ����
*@retval:��
*/

void KEY_Init(void)
{
    GPIO_InitTypeDef       GPIO_InitStruce;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruce.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruce.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStruce);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);

}

/************************
����ɨ��
��ڲ�����˫���ȴ�ʱ��
����ֵ����״̬��0�޶��� 1���� 2˫��
*************************/
uint8_t click_N_Double (u8 time)
{
    static u8 flag_key, count_key, double_key;
	static u16 count_single; 
    
	if(0 == KEY && 0 == flag_key)    flag_key = 1;
	
	if(0 == count_key)
	{
	  if(flag_key == 1)
	  {
	     double_key++;
		 count_key = 1;
	  }
	  
	  if(double_key == 2)
	  {
	     double_key = 0;
		 count_single = 0;
		  return 2; //˫��ִ������
	  }
	}
	if(1 == KEY)              flag_key = 0,count_key = 0;
	
	if(1 == double_key)
	{
	   if(++count_single > time)
	   {
	     double_key = 0;
		 count_single = 0;
		 return 1; //����ִ�е�ָ��
	   }
	
	}
	return 0;
}


/*******************************
����ɨ��
��ڲ�������
����ֵ��0�޶�����1����
********************************/

uint8_t click(void)
{
  static u8 flag_key = 1; //�����ɿ���־
	
	if(flag_key && KEY == 0)
	{
	  flag_key = 0;
		return 1; //��������
	}
	else if(1 == KEY)   flag_key = 1;
	
    return 0; //�ް�������
}
