#include "sys.h"

u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //蓝牙遥控相关的变量
u8 Flag_Stop=1,Flag_Show=1;
int Encoder_Left,Encoder_Right;                         //左右编码器的脉冲计数
int Moto1,Moto2;                                        //电机PWM变量
int Temperature;                                        //显示温度
int Voltage;                                            //电池电压采样相关的变量
float Angle_Balance,Gyro_Balance,Gyro_Turn;             //平衡倾角  平衡陀螺仪  转向陀螺仪
u32 Distance;                                           //超声波测距
int main(void)
{
    Stm32_Clock_Init(9);                                //系统时钟设置
    delay_init(72);                                       //延迟初始化
	JTAG_Set(JTAG_SWD_DISABLE);                         //关闭JTAG接口
	JTAG_Set(SWD_ENABLE);                               //打卡SWD接口
    led_init();
    uart_init(72,11520);
	uart3_init(9600);                                   //串口3初始化
    OLED_Init();
	Adc_Init();
	MiniBalance_PWM_Init(7199,0);                       //10HKZ高频可以防止电机低频时的尖叫声
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	IIC_Init();
	MPU6050_initialize();
	DMP_Init();
	Timer1_Init(49,7199);                              //5ms进入一次中断服务函数
    while(1)
    {   
		delay_ms(500);                                 //延时减缓数据传输频率，确保通信的稳定
		oled_show();
        //Led_Flash(10);
        delay_ms(100);		                           //延时减缓数据传输频率，确保通信的稳定           
    }
}
