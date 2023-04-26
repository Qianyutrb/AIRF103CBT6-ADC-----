#include "delay.h"
#include "oled.h"
#include "adc.h"
#include "main.h"
#include "pwm.h"
#include "stdio.h"

/* ��ʼ������Ƕ� */
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

	/* ��ȡ�ĸ�������״̬ */
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
 * @brief PID�㷨���㺯��
 * @param setpoint �趨ֵ
 * @param input ʵ��ֵ
 * @return PID���ֵ
 */
float pid_algorithm(float setpoint, float input)
{
    // PID����
    float Kp = 1.0;  // ����ϵ��
    float Ki = 0.1;  // ����ϵ��
    float Kd = 0.01; // ΢��ϵ��

    // PID����
    float error = setpoint - input; // ƫ��
    static float prev_error = 0;    // �ϴ�ƫ��
    static float integral = 0;      // ������
    float derivative = error - prev_error; // ΢����

    // ����PID���
    float output = Kp * error + Ki * integral + Kd * derivative;

    // ����PID����
    integral += error;    // �ۼӻ�����
    prev_error = error;   // �����ϴ�ƫ��

    // ����PID���
    return output;
}


void manua_task()
{
	char angle_str[6] = {0};

	float up_key_value = 0, down_key_value = 0, left_key_value = 0, right_key_value = 0;

	/* ��ȡ�ĸ�������״̬ */
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

	/* �� */
	if (up_key_value > 3.0f)
	{
		servo1_angle -= 10;
		if (servo1_angle >= 180)
		{
			servo1_angle = 180;
		}
		Servo_SetAngle(1, servo1_angle);
	}
	/* �� */
	else if (down_key_value > 3.0f)
	{
		servo1_angle += 10;
		if (servo1_angle <= 0)
		{
			servo1_angle = 0;
		}
		Servo_SetAngle(1, servo1_angle);
	}
	/* �� */
	if (left_key_value > 3.0f)
	{
		servo2_angle -= 10;
		if (servo2_angle >= 180)
		{
			servo2_angle = 180;
		}
		Servo_SetAngle(2, servo2_angle);
	}
	/* �� */
	else if (right_key_value > 3.0f)
	{
		servo2_angle += 10;
		if (servo2_angle <= 0)
		{
			servo2_angle = 0;
		}
		Servo_SetAngle(2, servo2_angle);
	}

	// ��OLED����ʾ��ǰ����Ƕ�

	OLED_ShowString(0, 0, "Servo1: ", 1);
	sprintf(angle_str, "%d", servo1_angle);
	OLED_ShowString(64, 0, angle_str, 1);
	OLED_ShowString(0, 2, "Servo2: ", 1);
	sprintf(angle_str, "%d", servo2_angle);
	OLED_ShowString(64, 2, angle_str, 1);
}

/**

@brief ʵ����Դ�Զ�׷Ѱ���ܵĺ���

@note �ú��������ĸ������������ɼ��� ADC ֵ�Ĵ�С���������������ת���Ƕȣ���׷����Դ

@note �����������Ų�Ϊ���������⣬�Ҿ���Ͻ������ͨ���Ƚ��ĸ������� ADC ֵ�Ĵ�С��ȷ�����ת���ĽǶ�

@param �޲���

@retval �޷���ֵ
*/
void auto_task(void)
{
	float up_value = 0, down_value = 0, left_value = 0, right_value = 0; // ���� ADC ֵ
	// uint8_t servo1_angle = 90, servo2_angle = 90;							// ��ʼ�����������ת���Ƕ�
	uint8_t flag = 0; // ת���־λ

	// ��ȡ�ĸ������������� ADC ֵ
	get_adc_Voltage(&up_value, &down_value, &left_value, &right_value);

	/* PID���� */
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

	// ���� ADC ֵ�Ĵ�С���������������ת���Ƕ�
	if (up_value > down_value && up_value > left_value && up_value > right_value) // ����
	{
		if (servo1_angle > 0 && flag == 0) // ���1��ʱ����ת
		{
			servo1_angle -= 5;	   // ÿ�μ��� 5 ��
			if (servo1_angle == 0) // �������ת��ʱ��ת���־λ��Ϊ 1
			{
				flag = 1;
			}
		}
		else if (servo1_angle < 180 && flag == 1) // ���1˳ʱ����ת
		{
			servo1_angle += 5;		 // ÿ������ 5 ��
			if (servo1_angle == 180) // �������ת��ʱ��ת���־λ��Ϊ 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(1, servo1_angle); // ���ƶ��1ת����ָ���Ƕ�
	}
	else if (down_value > up_value && down_value > left_value && down_value > right_value) // ����
	{
		if (servo1_angle < 180 && flag == 0) // ���1˳ʱ����ת
		{
			servo1_angle += 5;		 // ÿ������ 5 ��
			if (servo1_angle == 180) // �������ת��ʱ��ת���־λ��Ϊ 1
			{
				flag = 1;
			}
		}
		else if (servo1_angle > 0 && flag == 1) // ���1��ʱ����ת
		{
			servo1_angle -= 5;	   // ÿ�μ��� 5 ��
			if (servo1_angle == 0) // �������ת��ʱ��ת���־λ��Ϊ 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(1, servo1_angle); // ���ƶ��1ת����ָ���Ƕ�
	}
	else if (left_value > up_value && left_value > down_value && left_value > right_value) // ����
	{
		if (servo2_angle > 0 && flag == 0) // ���2��ʱ����ת
		{
			servo2_angle -= 5;	   // ÿ�μ��� 5 ��
			if (servo2_angle == 0) // �������ת��ʱ��ת���־λ��Ϊ 1
			{
				flag = 1;
			}
		}
		else if (servo2_angle < 180 && flag == 1) // ���2˳ʱ����ת
		{
			servo2_angle += 5;		 // ÿ������ 5 ��
			if (servo2_angle == 180) // �������ת��ʱ��ת���־λ��Ϊ 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(2, servo2_angle); // ���ƶ��2ת����ָ���Ƕ�
	}
	else if (right_value > up_value && right_value > down_value && right_value > left_value) // ����
	{
		if (servo2_angle < 180 && flag == 0) // ���2˳ʱ����ת
		{
			servo2_angle += 5;		 // ÿ������ 5 ��
			if (servo2_angle == 180) // �������ת��ʱ��ת���־λ��Ϊ 1
			{
				flag = 1;
			}
		}
		else if (servo2_angle > 0 && flag == 1) // ���2��ʱ����ת
		{
			servo2_angle -= 5;	   // ÿ�μ��� 5 ��
			if (servo2_angle == 0) // �������ת��ʱ��ת���־λ��Ϊ 0
			{
				flag = 0;
			}
		}
		Servo_SetAngle(2, servo2_angle); // ���ƶ��2ת����ָ���Ƕ�
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Delay_Init();
	Servo_GPIO_Init();
	Servo_TIM_Init();
    OLEDInit();
	OLED_Clear();
	Adc_Init();
	
	/* ��ʼ�����Ϊ90�� */
	Servo_SetAngle(1, servo1_angle);
	Servo_SetAngle(2, servo2_angle);

	while (1)
	{
		check_mode();

		if (e_AUTO_MODE == g_mode)
		{
            OLED_ShowString(0, 4, "AUTO MODE", 1);
			auto_task(); // �Զ�ģʽ
		}
		else
		{
            OLED_ShowString(0, 4, "MANUAL MODE", 1);
			manua_task(); // �ֶ�ģʽ
		}
	}
}
