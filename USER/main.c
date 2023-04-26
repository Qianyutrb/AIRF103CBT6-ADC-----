#include "delay.h"
#include "oled.h"
#include "adc.h"
#include "main.h"
#include "pwm.h"
#include "stdio.h"

/* 初始化舵机角度 */
uint16_t servo1_angle = 90;
uint16_t servo2_angle = 90;
uint8_t  g_mode = e_MANUAL_MODE;

void get_key_value(float *up_key_value, float *down_key_value, float *left_key_value, float *right_key_value)
{

	*up_key_value = Get_Voltage(ADC_Channel_4, 10);
	*down_key_value = Get_Voltage(ADC_Channel_5, 10);
	*left_key_value = Get_Voltage(ADC_Channel_8, 10);
	*right_key_value = Get_Voltage(ADC_Channel_9, 10);
    
}

void check_mode(void)
{

	float up_key_value = 0, down_key_value = 0, left_key_value = 0, right_key_value = 0;

	/* 获取四个按键的状态 */
	get_key_value(&up_key_value, &down_key_value, &left_key_value, &right_key_value);

	if (up_key_value > 3.0f && down_key_value > 3.0f)
	{
		if (g_mode == e_AUTO_MODE)
		{
			g_mode = e_MANUAL_MODE;
		}

		else
		{
			g_mode = e_AUTO_MODE;
		}
	}
}

/* void get_adc_value(u16 *up_value, u16 *down_value, u16 *left_value, u16 *right_value)
{

	*up_value = Get_Adc_Average(ADC_Channel_0, 10);
	*down_value = Get_Adc_Average(ADC_Channel_1, 10);
	*left_value = Get_Adc_Average(ADC_Channel_2, 10);
	*right_value = Get_Adc_Average(ADC_Channel_3, 10);
} */


void get_adc_Voltage(float *up_value, float *down_value, float *left_value, float *right_value)
{

	*up_value = Get_Voltage(ADC_Channel_0, 10);
	*down_value = Get_Voltage(ADC_Channel_1, 10);
	*left_value = Get_Voltage(ADC_Channel_2, 10);
	*right_value = Get_Voltage(ADC_Channel_3, 10);
}

/**
 * @brief PID算法计算函数
 * @param setpoint 设定值
 * @param input 实际值
 * @return PID输出值
 */
float pid_algorithm(float setpoint, float input)
{
    // PID参数
    float Kp = 1.0;  // 比例系数
    float Ki = 0.1;  // 积分系数
    float Kd = 0.01; // 微分系数

    // PID变量
    float error = setpoint - input; // 偏差
    static float prev_error = 0;    // 上次偏差
    static float integral = 0;      // 积分项
    float derivative = error - prev_error; // 微分项

    // 计算PID输出
    float output = Kp * error + Ki * integral + Kd * derivative;

    // 更新PID变量
    integral += error;    // 累加积分项
    prev_error = error;   // 保存上次偏差

    // 返回PID输出
    return output;
}


void manua_task()
{
	char angle_str[6] = {0};

	float up_key_value = 0, down_key_value = 0, left_key_value = 0, right_key_value = 0;

	/* 获取四个按键的状态 */
	get_key_value(&up_key_value, &down_key_value, &left_key_value, &right_key_value);

//	OLED_ShowString(0, 0, "up: ", 1);
//	sprintf(angle_str, "%.3f V", up_key_value);
//	OLED_ShowString(65, 0, angle_str, 1);
//	OLED_ShowString(0, 2, "down: ", 1);
//	sprintf(angle_str, "%.3f V", down_key_value);
//	OLED_ShowString(65, 2, angle_str, 1);
//	OLED_ShowString(0, 4, "left: ", 1);
//	sprintf(angle_str, "%.3f V", left_key_value);
//	OLED_ShowString(65, 4, angle_str, 1);
//	OLED_ShowString(0, 6, "right: ", 1);
//	sprintf(angle_str, "%.3f V", right_key_value);
//	OLED_ShowString(65, 6, angle_str, 1);

	/* 上 */
	if (up_key_value > 3.0f)
	{
		servo1_angle -= 10;
		if (servo1_angle >= 180)
		{
			servo1_angle = 180;
		}
		Servo_SetAngle(1, servo1_angle);
	}
	/* 下 */
	else if (down_key_value > 3.0f)
	{
		servo1_angle += 10;
		if (servo1_angle <= 0)
		{
			servo1_angle = 0;
		}
		Servo_SetAngle(1, servo1_angle);
	}
	/* 左 */
	if (left_key_value > 3.0f)
	{
		servo2_angle -= 10;
		if (servo2_angle >= 180)
		{
			servo2_angle = 180;
		}
		Servo_SetAngle(2, servo2_angle);
	}
	/* 右 */
	else if (right_key_value > 3.0f)
	{
		servo2_angle += 10;
		if (servo2_angle <= 0)
		{
			servo2_angle = 0;
		}
		Servo_SetAngle(2, servo2_angle);
	}

	// 在OLED上显示当前舵机角度

	OLED_ShowString(0, 0, "Servo1: ", 1);
	sprintf(angle_str, "%d", servo1_angle);
	OLED_ShowString(64, 0, angle_str, 1);
	OLED_ShowString(0, 2, "Servo2: ", 1);
	sprintf(angle_str, "%d", servo2_angle);
	OLED_ShowString(64, 2, angle_str, 1);
}

/**

@brief 实现声源自动追寻功能的函数

@note 该函数根据四个声音传感器采集的 ADC 值的大小，调整两个舵机的转动角度，以追踪声源

@note 声音传感器排布为正方形向外，且距离较近，因此通过比较四个传感器 ADC 值的大小来确定舵机转动的角度

@param 无参数

@retval 无返回值
*/
void auto_task(void)
{
	float up_value = 0, down_value = 0, left_value = 0, right_value = 0; // 定义 ADC 值
	// uint8_t servo1_angle = 90, servo2_angle = 90;							// 初始化两个舵机的转动角度
	uint8_t flag = 0; // 转向标志位

	// 获取四个声音传感器的 ADC 值
	get_adc_Voltage(&up_value, &down_value, &left_value, &right_value);

	/* PID处理 */
	up_value = pid_algorithm(50.0, up_value);
	down_value = pid_algorithm(50.0, down_value);
	left_value = pid_algorithm(50.0, left_value);
	right_value = pid_algorithm(50.0, right_value);

	char angle_str[6] = {0};

	/*OLED_ShowString(0, 0, "up: ", 1);
	sprintf(angle_str, "%.3f V", up_value);
	OLED_ShowString(65, 0, angle_str, 1);
	OLED_ShowString(0, 2, "down: ", 1);
	sprintf(angle_str, "%.3f V", down_value);
	OLED_ShowString(65, 2, angle_str, 1);
	OLED_ShowString(0, 4, "left: ", 1);
	sprintf(angle_str, "%.3f V", left_value);
	OLED_ShowString(65, 4, angle_str, 1);
	OLED_ShowString(0, 6, "right: ", 1);
	sprintf(angle_str, "%.3f V", right_value);
	OLED_ShowString(65, 6, angle_str, 1); */

	// 根据 ADC 值的大小，调整两个舵机的转动角度
	if (up_value > down_value && up_value > left_value && up_value > right_value) // 向上
	{
		if (servo1_angle > 0 && flag == 0) // 舵机1逆时针旋转
		{
			servo1_angle -= 5;	   // 每次减少 5 度
			if (servo1_angle == 0) // 到达最大转角时，转向标志位设为 1
			{
				flag = 1;
			}
		}
		else if (servo1_angle < 180 && flag == 1) // 舵机1顺时针旋转
		{
			servo1_angle += 5;		 // 每次增加 5 度
			if (servo1_angle == 180) // 到达最大转角时，转向标志位设为 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(1, servo1_angle); // 控制舵机1转动到指定角度
	}
	else if (down_value > up_value && down_value > left_value && down_value > right_value) // 向下
	{
		if (servo1_angle < 180 && flag == 0) // 舵机1顺时针旋转
		{
			servo1_angle += 5;		 // 每次增加 5 度
			if (servo1_angle == 180) // 到达最大转角时，转向标志位设为 1
			{
				flag = 1;
			}
		}
		else if (servo1_angle > 0 && flag == 1) // 舵机1逆时针旋转
		{
			servo1_angle -= 5;	   // 每次减少 5 度
			if (servo1_angle == 0) // 到达最大转角时，转向标志位设为 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(1, servo1_angle); // 控制舵机1转动到指定角度
	}
	else if (left_value > up_value && left_value > down_value && left_value > right_value) // 向左
	{
		if (servo2_angle > 0 && flag == 0) // 舵机2逆时针旋转
		{
			servo2_angle -= 5;	   // 每次减少 5 度
			if (servo2_angle == 0) // 到达最大转角时，转向标志位设为 1
			{
				flag = 1;
			}
		}
		else if (servo2_angle < 180 && flag == 1) // 舵机2顺时针旋转
		{
			servo2_angle += 5;		 // 每次增加 5 度
			if (servo2_angle == 180) // 到达最大转角时，转向标志位设为 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(2, servo2_angle); // 控制舵机2转动到指定角度
	}
	else if (right_value > up_value && right_value > down_value && right_value > left_value) // 向右
	{
		if (servo2_angle < 180 && flag == 0) // 舵机2顺时针旋转
		{
			servo2_angle += 5;		 // 每次增加 5 度
			if (servo2_angle == 180) // 到达最大转角时，转向标志位设为 1
			{
				flag = 1;
			}
		}
		else if (servo2_angle > 0 && flag == 1) // 舵机2逆时针旋转
		{
			servo2_angle -= 5;	   // 每次减少 5 度
			if (servo2_angle == 0) // 到达最大转角时，转向标志位设为 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(2, servo2_angle); // 控制舵机2转动到指定角度
	}
    OLED_ShowString(0, 0, "Servo1: ", 1);
	sprintf(angle_str, "%d", servo1_angle);
	OLED_ShowString(64, 0, angle_str, 1);
	OLED_ShowString(0, 2, "Servo2: ", 1);
	sprintf(angle_str, "%d", servo2_angle);
	OLED_ShowString(64, 2, angle_str, 1); 
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Delay_Init();
	Servo_GPIO_Init();
	Servo_TIM_Init();
    OLEDInit();
	OLED_Clear();
	Adc_Init();
	
	/* 初始化舵机为90度 */
	Servo_SetAngle(1, servo1_angle);
	Servo_SetAngle(2, servo2_angle);

	while (1)
	{
		check_mode();

		if (e_AUTO_MODE == g_mode)
		{
            OLED_ShowString(0, 4, "AUTO MODE", 1);
			auto_task(); // 自动模式
		}
		else
		{
            OLED_ShowString(0, 4, "MANUAL MODE", 1);
			manua_task(); // 手动模式
		}
	}
}
