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
功能：直立环PD控制  Kp*Ek+Kd*Ek_D
参数：角度，角加速度
返回值：直立控制的PWM控制电机
***************************************************/
int balance(float Angle, float Gyro)
{
    float Bias, kp = 550, kd = 1.5;
    int balance;
    Bias = Angle - Mid;                    //求出平衡的角度中值和机械相关 -0意味着身重中心在0度附近

    balance = kp * Bias + Gyro * kd; //计算平衡控制电机PWM  PD控制 kp是p系数 kd是D系数
    return balance;                       //Gyro-0中的0就是原始角加速度
}

/**************************************************
速度环PI：kp*Ek+ki*Ek_S
功能：速度PI控制 修改前进后退速度，请修改Movement
参数：左轮编码器，右轮编码器
**************************************************/
int velocity(int encoder_left, int encoder_right)
{
    static float Velocity, Encoder_Least, Encoder, Movement;
    static float Encoder_Integral;
    float kp = -120, ki = -0.6;
    static float last_Encoder;

    //==================遥控前进后退部分================//
    if(1 == Flag_Qian)   Movement = 300 / Flag_sudu;
    else if(1 == Flag_Hou)    Movement = -250 / Flag_sudu;
    else  Movement = 0;

    //=================速度PI控制器=====================//
    Encoder_Least = (Encoder_Left + Encoder_Right) - 0;     //获取最新速度偏差==测量速度（左右编码器之和 -目标速度（此处为0） ）
    Encoder = Encoder_Least * 0.1 + last_Encoder * 0.9;     //一阶低通滤波器
    Encoder_Integral += Encoder;
    Encoder_Integral = Encoder_Integral - Movement;

    //=================积分限幅=========================//
    if(Encoder_Integral > 10000)     Encoder_Integral = 10000;

    if(Encoder_Integral < -10000)     Encoder_Integral = -10000;


    Velocity = Encoder * kp + Encoder_Integral * ki;
    last_Encoder = Encoder;

    if(Turn_Off(Angle_Balance) == 1 || Flag_Stop == 1)   Encoder_Integral = 0;

    return Velocity;


}
/*********************************************************
功能：转向PD控制
参数：左右轮编码器，z轴陀螺仪
返回值：转向控制PWM
*********************************************************/
int turn(int encoder_left, int encoder_right, float gyro)
{
    static float Turn_Target, Turn, Encoder_temp, Turn_Convert = 0.9, Turn_Count;
    float Turn_Amplitude = 150 / Flag_sudu, Kp = 42, Kd = 0;

    //===================遥控左右旋转部分=====================//
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

    if(Turn_Target > Turn_Amplitude)  Turn_Target = Turn_Amplitude; //===转向速度限幅

    if(Turn_Target < -Turn_Amplitude) Turn_Target = -Turn_Amplitude;

    if(Flag_Qian == 1 || Flag_Hou == 1)  Kd = 1;
    else Kd = 0; //转向的时候取消陀螺仪的纠正 有点模糊PID的思想

    //======================转向PD控制===========================//
    Turn = -Turn_Target * Kp - gyro * Kd;
    return Turn;
}

/***************************************************
功能：赋值给PWM寄存器
参数：左轮PWM，右轮PWM
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
功能：限幅PWM赋值
***************************************************/
void Xianfu_Pwm(void)
{
    int Amplitude = 7100;    //pwm满幅7200，限幅7100

    if(Moto1 < -Amplitude) Moto1 = -Amplitude;

    if(Moto1 >  Amplitude) Moto1 =  Amplitude;

    if(Moto2 < -Amplitude) Moto2 = -Amplitude;

    if(Moto2 >  Amplitude) Moto1 =  Amplitude;

}

/**************************************************
功能：异常关闭电机
参数：倾角和电压
返回值：1异常2正常
**************************************************/
u8 Turn_Off(float angle)
{
    u8 temp;

    if(angle < -40 || angle > 40 || Flag_Stop == 1)
    {
        //倾角大于40度关闭电机
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
功能：获取角度
卡尔曼滤波
*****************************************************/
void Get_Angle(void)
{
    float Accel_Y, Accel_X, Accel_Z, Gyro_Y,  Gyro_Z;
    Gyro_Y = (I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_YOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_YOUT_L); //读取Y轴陀螺仪
    Gyro_Z = (I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_ZOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_GYRO_ZOUT_L); //读取Z轴陀螺仪
    Accel_X = (I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_XOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度记
    Accel_Z = (I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_ZOUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度记
    
	if(Gyro_Y > 32768)  Gyro_Y -= 65536; //数据类型转换  也可通过short强制类型转换

    if(Gyro_Z > 32768)  Gyro_Z -= 65536; //数据类型转换

    if(Accel_X > 32768) Accel_X -= 65536; //数据类型转换

    if(Accel_Z > 32768) Accel_Z -= 65536; //数据类型转换
	
	Gyro_Balance = -Gyro_Y;                                //更新平衡角速度
    Accel_Y = atan2(Accel_X, Accel_Z) * 180 / PI;          //计算与地面的夹角，得到重力加速度
    Gyro_Y = Gyro_Y / 16.4;                                //陀螺仪量程转换
    Kalman_Filter(Accel_Y, -Gyro_Y);                       //卡尔曼滤波获取最佳的角度值
    Angle_Balance = angle;                                 //更新平衡倾角
    Gyro_Turn = Gyro_Z;                                    //更新转向角速度
    Acceleration_Z = Accel_Z;                              //===更新Z轴加速度计
}


/*************************************************
功能：绝对值函数
*************************************************/
int myabs(int a)
{
    int  temp;

    if(a < 0) temp = -a;
    else  temp = a;

    return temp;
}


