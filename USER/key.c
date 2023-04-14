// #include "air32f10x.h"
// #include "air32f10x_gpio.h"
// #include "air32f10x_rcc.h"
// #include "delay.h"
// #include "oled.h"

// //#define ROW1 GPIO_Pin_4
// //#define ROW2 GPIO_Pin_5
// //#define ROW3 GPIO_Pin_8
// //#define ROW4 GPIO_Pin_9

// //#define COL1 GPIO_Pin_0
// //#define COL2 GPIO_Pin_1
// //#define COL3 GPIO_Pin_3
// //#define COL4 GPIO_Pin_4

// #define ROW_NUM 4 // 矩阵键盘的行数
// #define COL_NUM 4 // 矩阵键盘的列数

// static const uint8_t key_map[ROW_NUM][COL_NUM] = {
//     {1, 2, 3, 4},
//     {5, 6, 7, 8},
//     {9, 10, 11, 12},
//     {13, 0, 14, 15}

// };

// void key_init_gpio(void)
// {
//     GPIO_InitTypeDef GPIO_InitStructure = {0};
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

//     // 设置行引脚为推挽输出模式
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_Init(GPIOA, &GPIO_InitStructure);
//     GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9); // 将当前列设置为高电平，继续扫描下一列

//     // 设置列引脚为上拉输入模式
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB, &GPIO_InitStructure);
// }

// uint8_t get_key_value(void)
// {
//     static const uint16_t ROW_PINS[ROW_NUM] = {GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_8, GPIO_Pin_9};
//     static const uint16_t COL_PINS[COL_NUM] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_3, GPIO_Pin_4};
//     static const uint8_t KEY_MAP[ROW_NUM][COL_NUM] = {
//         {1, 2, 3, 0xC},
//         {4, 5, 6, 0xD},
//         {7, 8, 9, 0xE},
//         {0xA, 0, 0xB, 0xF}};

//     uint8_t key_value = 0;
//     GPIO_InitTypeDef GPIO_InitStructure = {0};

//     // 扫描每一列的状态
//     for (int i = 0; i < COL_NUM; i++)
//     {
//         GPIO_ResetBits(GPIOB, COL_PINS[i]); // 将当前列设置为低电平
//         GPIO_InitStructure.GPIO_Pin = COL_PINS[i];
//         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//         GPIO_Init(GPIOB, &GPIO_InitStructure);

//         // 扫描每一行的状态
//         for (int j = 0; j < ROW_NUM; j++)
//         {
//             GPIO_InitStructure.GPIO_Pin = ROW_PINS[j];
//             GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//             GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//             GPIO_Init(GPIOA, &GPIO_InitStructure);

//             if (GPIO_ReadInputDataBit(GPIOA, ROW_PINS[j]) == RESET)
//             {                 // 检测到按键按下
//                 Delay_Ms(50); // 延时一段时间，以消除机械弹跳产生的干扰

//                 if (GPIO_ReadInputDataBit(GPIOA, ROW_PINS[j]) == RESET)
//                 { // 再次检测到按键按下
//                     // 等待按键松开
//                     while (GPIO_ReadInputDataBit(GPIOA, ROW_PINS[j]) == RESET)
//                         ;
//                     key_value = KEY_MAP[j][i];
//                     goto end_scan;
//                 }
//             }
//         }

//         GPIO_InitStructure.GPIO_Pin = COL_PINS[i];
//         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//         GPIO_Init(GPIOB, &GPIO_InitStructure);
//     }
//     OLED_ShowNum(0, 4, key_value, 1, 1);
//     return key_value;
// end_scan:
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//     for (int i = 0; i < ROW_NUM; i++)
//     {
//         GPIO_InitStructure.GPIO_Pin = ROW_PINS[i];
//         GPIO_Init(GPIOA, &GPIO_InitStructure);
//     }
//     for (int i = 0; i < COL_NUM; i++)
//     {
//         GPIO_InitStructure.GPIO_Pin = COL_PINS[i];
//         GPIO_Init(GPIOB, &GPIO_InitStructure);
//     }
//     OLED_ShowNum(0, 4, key_value, 1, 1);
//     return key_value;
// }
