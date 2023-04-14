#include "air32f10x.h"
#include "air32f10x_gpio.h"
#include "air32f10x_tim.h"
#include "pwm.h"

void Servo_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA6和PA7引脚为推挽输出
    GPIO_InitStruct.GPIO_Pin = SERVO1_PIN | SERVO2_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO1_PORT, &GPIO_InitStruct);
}

void Servo_TIM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 配置TIM3为PWM模式
    TIM_TimeBaseStructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_Period = SERVO_PWM_PERIOD;
    TIM_InitStruct.TIM_Prescaler = SystemCoreClock / 2000000 - 1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);

    // 配置TIM3通道1和通道2为PWM输出模式
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = SERVO_PWM_DUTY_MIN;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM_OC2Init(TIM3, &TIM_OCInitStruct);

    // 使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}

void Servo_SetAngle(uint8_t servo_num, uint16_t angle)
{
    uint16_t duty_cycle;

    // 将角度转换为对应的PWM输出占空比值
    duty_cycle = (uint16_t)(angle * (SERVO_PWM_DUTY_MAX - SERVO_PWM_DUTY_MIN) / 180 + SERVO_PWM_DUTY_MIN);

    if (servo_num == 1)
    {
        TIM_SetCompare1(TIM3, duty_cycle);
    }
    else if (servo_num == 2)
    {
        TIM_SetCompare2(TIM3, duty_cycle);
    }
}
