#ifndef __PWM_H
#define __PWM_H
#include "air32f10x.h"

#define SERVO1_PIN GPIO_Pin_6
#define SERVO1_PORT GPIOA
#define SERVO2_PIN GPIO_Pin_7
#define SERVO2_PORT GPIOA

#define SERVO_PWM_FREQ 50                                             // PWM频率为50Hz
#define SERVO_PWM_PERIOD ((SystemCoreClock / 2) / SERVO_PWM_FREQ - 1) // 计算PWM周期值
#define SERVO_PWM_DUTY_MIN 500                                        // PWM输出最小值
#define SERVO_PWM_DUTY_MAX 2500                                       // PWM输出最大值

void Servo_GPIO_Init(void);
void Servo_TIM_Init(void);
void Servo_SetAngle(uint8_t servo_num, uint16_t angle);
#endif
