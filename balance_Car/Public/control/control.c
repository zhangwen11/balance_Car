#include "control.h"
#include "filter.h"

#define Mid 1
u8 Flag_Target;
float Acceleration_Z;
int Balance_Pwm, Velocity_Pwm, Turn_Pwm;
int TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        Flag_Target = !Flag_Target;

        if(Flag_Target == 1)
        {
            Get_Angle();
            return 0;
        }
		
		Encoder_Left = -Read_Encoder(2);
		Encoder_Right = Read_Encoder(4);
		Get_Angle();
		Voltage = Get_battery_volt();
		Balance_Pwm = balance(Angle_Balance,Gyro_Balance);
		Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);
		Turn_Pwm = turn(Encoder_Left,Encoder_Right,Gyro_Turn);
		Moto1 = Balance_Pwm - Velocity_Pwm - Turn_Pwm;
		Moto2 = Balance_Pwm - Velocity_Pwm + Turn_Pwm;
		Xianfu_Pwm();
		
		if(Turn_Off(Angle_Balance) == 0)
			Set_Pwm(Moto1,Moto2);

    }
	
        return 0;
}

/***************************************************
���ܣ�ֱ����PD����  Kp*Ek+Kd*Ek_D
�������Ƕȣ��Ǽ��ٶ�
����ֵ��ֱ�����Ƶ�PWM���Ƶ��
***************************************************/
int balance(float Angle, float Gyro)
{
    float Bias, kp = 550, kd = 1.5;
    int balance;
    Bias = Angle - Mid;                    //���ƽ��ĽǶ���ֵ�ͻ�е��� -0��ζ������������0�ȸ���

    balance = kp * Bias + Gyro * kd; //����ƽ����Ƶ��PWM  PD���� kp��pϵ�� kd��Dϵ��
    return balance;                       //Gyro-0�е�0����ԭʼ�Ǽ��ٶ�
}

/**************************************************
�ٶȻ�PI��kp*Ek+ki*Ek_S
���ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ����޸�Movement
���������ֱ����������ֱ�����
**************************************************/
int velocity(int encoder_left, int encoder_right)
{
    static float Velocity, Encoder_Least, Encoder, Movement;
    static float Encoder_Integral;
    float kp = -120, ki = -0.6;
    static float last_Encoder;

    //==================ң��ǰ�����˲���================//
    if(1 == Flag_Qian)   Movement = 300 / Flag_sudu;
    else if(1 == Flag_Hou)    Movement = -250 / Flag_sudu;
    else  Movement = 0;

    //=================�ٶ�PI������=====================//
    Encoder_Least = (Encoder_Left + Encoder_Right) - 0;     //��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�� -Ŀ���ٶȣ��˴�Ϊ0�� ��
    Encoder = Encoder_Least * 0.1 + last_Encoder * 0.9;     //һ�׵�ͨ�˲���
    Encoder_Integral += Encoder;
    Encoder_Integral = Encoder_Integral - Movement;

    //=================�����޷�=========================//
    if(Encoder_Integral > 10000)     Encoder_Integral = 10000;

    if(Encoder_Integral < -10000)     Encoder_Integral = -10000;


    Velocity = Encoder * kp + Encoder_Integral * ki;
    last_Encoder = Encoder;

    if(Turn_Off(Angle_Balance) == 1 || Flag_Stop == 1)   Encoder_Integral = 0;

    return Velocity;


}
/*********************************************************
���ܣ�ת��PD����
�����������ֱ�������z��������
����ֵ��ת�����PWM
*********************************************************/
int turn(int encoder_left, int encoder_right, float gyro)
{
    static float Turn_Target, Turn, Encoder_temp, Turn_Convert = 0.9, Turn_Count;
    float Turn_Amplitude = 150 / Flag_sudu, Kp = 42, Kd = 0;

    //===================ң��������ת����=====================//
    if(1 == Flag_Left || 1 == Flag_Right)
    {
        if(++Turn_Count == 1)
            Encoder_temp = myabs(encoder_left + encoder_right);

        Turn_Convert = 50 / Encoder_temp;

        if(Turn_Convert < 0.6) Turn_Convert = 0.6;

        if(Turn_Convert > 3)Turn_Convert = 3;
    }
    else
    {
        Turn_Convert = 0.9;
        Turn_Count = 0;
        Encoder_temp = 0;
    }

    if(1 == Flag_Left)	           Turn_Target -= Turn_Convert;
    else if(1 == Flag_Right)	     Turn_Target += Turn_Convert;
    else Turn_Target = 0;

    if(Turn_Target > Turn_Amplitude)  Turn_Target = Turn_Amplitude; //===ת���ٶ��޷�

    if(Turn_Target < -Turn_Amplitude) Turn_Target = -Turn_Amplitude;

    if(Flag_Qian == 1 || Flag_Hou == 1)  Kd = 1;
    else Kd = 0; //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��

    //======================ת��PD����===========================//
    Turn = -Turn_Target * Kp - gyro * Kd;
    return Turn;
}

/***************************************************
���ܣ���ֵ��PWM�Ĵ���
����������PWM������PWM
***************************************************/
void  Set_Pwm(int moto1, int moto2)
{
    if(moto1 > 0)        AIN2 = 1,                  AIN1 = 0;
    else           AIN2 = 0,                    AIN1 = 1;

    PWMA = myabs(moto1);

    if(moto2 > 0)     BIN1 = 0,                   BIN2 = 1;
    else           BIN1 = 1,                   BIN2 = 0;

    PWMB = myabs(moto2);
}

/***************************************************
���ܣ��޷�PWM��ֵ
***************************************************/
void Xianfu_Pwm(void)
{
    int Amplitude = 7100;    //pwm����7200���޷�7100

    if(Moto1 < -Amplitude) Moto1 = -Amplitude;

    if(Moto1 >  Amplitude) Moto1 =  Amplitude;

    if(Moto2 < -Amplitude) Moto2 = -Amplitude;

    if(Moto2 >  Amplitude) Moto1 =  Amplitude;

}

/**************************************************
���ܣ��쳣�رյ��
��������Ǻ͵�ѹ
����ֵ��1�쳣2����
**************************************************/
u8 Turn_Off(float angle)
{
    u8 temp;

    if(angle < -40 || angle > 40 || Flag_Stop == 1)
    {
        //��Ǵ���40�ȹرյ��
        temp = 1;
        AIN1 = 0;
        AIN2 = 0;
        BIN1 = 0;
        BIN2 = 0;
    }
    else
        temp = 0;

    Flag_Stop = 0;
    return temp;

}

/*****************************************************
���ܣ���ȡ�Ƕ�
�������˲�
*****************************************************/
void Get_Angle(void)
{
    float Accel_Y, Accel_X, Accel_Z, Gyro_Y,  Gyro_Z;
    Gyro_Y = (I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_YOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_YOUT_L); //��ȡY��������
    Gyro_Z = (I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_ZOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_ZOUT_L); //��ȡZ��������
    Accel_X = (I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_XOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
    Accel_Z = (I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_ZOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
    
	if(Gyro_Y > 32768)  Gyro_Y -= 65536; //��������ת��  Ҳ��ͨ��shortǿ������ת��

    if(Gyro_Z > 32768)  Gyro_Z -= 65536; //��������ת��

    if(Accel_X > 32768) Accel_X -= 65536; //��������ת��

    if(Accel_Z > 32768) Accel_Z -= 65536; //��������ת��
	
	Gyro_Balance = -Gyro_Y;                                //����ƽ����ٶ�
    Accel_Y = atan2(Accel_X, Accel_Z) * 180 / PI;          //���������ļнǣ��õ��������ٶ�
    Gyro_Y = Gyro_Y / 16.4;                                //����������ת��
    Kalman_Filter(Accel_Y, -Gyro_Y);                       //�������˲���ȡ��ѵĽǶ�ֵ
    Angle_Balance = angle;                                 //����ƽ�����
    Gyro_Turn = Gyro_Z;                                    //����ת����ٶ�
    Acceleration_Z = Accel_Z;                              //===����Z����ٶȼ�
}


/*************************************************
���ܣ�����ֵ����
*************************************************/
int myabs(int a)
{
    int  temp;

    if(a < 0) temp = -a;
    else  temp = a;

    return temp;
}


