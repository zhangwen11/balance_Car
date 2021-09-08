#include "show.h"

unsigned char i;            //��������
unsigned char Send_Count;   //������Ҫ���͵����ݸ���

/***************************
���ܣ�OLED��ʾ
***************************/
void oled_show(void)
{
    OLED_Display_On();
    //===================��ʾ�˲���================//
    OLED_ShowString(00, 0, "made by zw");

    //==================��ʾ������1================//
    OLED_ShowString(00, 25, "Enco1:");

    if(Encoder_Left < 0)    OLED_ShowString(45, 25, "-"),
        OLED_ShowNumber(65, 25, -Encoder_Left, 5, 12);
    else   OLED_ShowString(45, 25, "+"),
               OLED_ShowNumber(65, 25, Encoder_Left, 5, 12);

    //================��ʾ������2=================//
    OLED_ShowString(00, 35, "Enco2:");

    if(Encoder_Right < 0)    OLED_ShowString(45, 35, "-"),
        OLED_ShowNumber(65, 35, -Encoder_Right, 5, 12);
    else    OLED_ShowString(45, 35, "+"),
                OLED_ShowNumber(65, 35, Encoder_Right, 5, 12);

    //===============��ʾ��ѹ====================//
    OLED_ShowString(00, 45, "Volta");
    OLED_ShowString(58, 45, ".");
    OLED_ShowString(80, 45, "V");
    OLED_ShowNumber(45, 45, Voltage / 100, 2, 12);
    OLED_ShowNumber(68, 45, Voltage % 100, 2, 12);

    if(Voltage % 10 < 10)  OLED_ShowNumber(62, 45, 0, 2, 12);

    //===========��ʾ�Ƕ�===========================//
    OLED_ShowString(0, 15, "Angle:");

    if(Angle_Balance < 0)      OLED_ShowNumber(50, 15, Angle_Balance + 360, 3, 16);
    else                         OLED_ShowNumber(50, 15, Angle_Balance, 3, 16);

    //============ˢ��=============================//
    OLED_Refresh_Gram();


}
