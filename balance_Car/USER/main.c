#include "sys.h"

u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //����ң����صı���
u8 Flag_Stop=1,Flag_Show=1;
int Encoder_Left,Encoder_Right;                         //���ұ��������������
int Moto1,Moto2;                                        //���PWM����
int Temperature;                                        //��ʾ�¶�
int Voltage;                                            //��ص�ѹ������صı���
float Angle_Balance,Gyro_Balance,Gyro_Turn;             //ƽ�����  ƽ��������  ת��������
u32 Distance;                                           //���������
int main(void)
{
    Stm32_Clock_Init(9);                                //ϵͳʱ������
    delay_init(72);                                       //�ӳٳ�ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);                         //�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);                               //��SWD�ӿ�
    led_init();
    uart_init(72,11520);
	uart3_init(9600);                                   //����3��ʼ��
    OLED_Init();
	Adc_Init();
	MiniBalance_PWM_Init(7199,0);                       //10HKZ��Ƶ���Է�ֹ�����Ƶʱ�ļ����
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	IIC_Init();
	MPU6050_initialize();
	DMP_Init();
	Timer1_Init(49,7199);                              //5ms����һ���жϷ�����
    while(1)
    {   
		delay_ms(500);                                 //��ʱ�������ݴ���Ƶ�ʣ�ȷ��ͨ�ŵ��ȶ�
		oled_show();
        //Led_Flash(10);
        delay_ms(100);		                           //��ʱ�������ݴ���Ƶ�ʣ�ȷ��ͨ�ŵ��ȶ�           
    }
}
