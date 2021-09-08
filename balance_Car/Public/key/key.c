#include "key.h"

/*@brief:按键初始化
*@param ：无
*@retval:无
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
按键扫描
入口参数：双击等待时间
返回值按键状态：0无动作 1单机 2双击
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
		  return 2; //双击执行命令
	  }
	}
	if(1 == KEY)              flag_key = 0,count_key = 0;
	
	if(1 == double_key)
	{
	   if(++count_single > time)
	   {
	     double_key = 0;
		 count_single = 0;
		 return 1; //单击执行的指令
	   }
	
	}
	return 0;
}


/*******************************
按键扫描
入口参数：无
返回值：0无动作，1单击
********************************/

uint8_t click(void)
{
  static u8 flag_key = 1; //按键松开标志
	
	if(flag_key && KEY == 0)
	{
	  flag_key = 0;
		return 1; //按键按下
	}
	else if(1 == KEY)   flag_key = 1;
	
    return 0; //无按键按下
}
