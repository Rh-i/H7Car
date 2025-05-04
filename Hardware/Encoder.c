#include "Encoder.h"
// 此处记录了编码器的初始化和更新函数
// 以及速度采样时间的设置
// 编码器对应参数需要修改 编码器的分辨率
// 需要用个中断来调用Encoder_Update()来更新
// 状态机的Task_solver 在这个地方 直接改这个去实现就行

// LPTIM2_IN1 IN2 是用旋钮编码器控制模式的 左接1 右接2  顺时针拧为正方向进行 1 2 3模式操控

Encoder encoder_L1, encoder_L2;
Encoder encoder_R1, encoder_R2;

volatile uint8_t Task_Flag = 0;
volatile uint8_t Task_Count = 1;

void Encoder_Init()
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // L1 启动编码器
    // HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // L2
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // R1
    // HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL); // R2
    HAL_LPTIM_Encoder_Start(&hlptim2, 0);

    __HAL_TIM_SET_COUNTER(&htim3, 32767); // 清空定时器的计数值，以便下一次采样
    // __HAL_TIM_SET_COUNTER(&htim4, 32767);
    __HAL_TIM_SET_COUNTER(&htim5, 32767);
    // __HAL_TIM_SET_COUNTER(&htim8, 32767);
}

void Encoder_Update(Encoder *enc, TIM_HandleTypeDef *htim) // 编码器更新函数 结果通过指针存储在结构体中 左- 右+ 方向问题要注意 自己debug一下就好
// 注意 编码器的计数方向与其中电机的方向相反 所以速度计算时要注意 CNT默认设置65535 所以其中设置CNT为32767
// 我这里的计算方式是每次都用记录值-32767 然后把计数器的值改到32767    避免0的时候无法读到反转
{
    enc->raw_count = (int32_t)__HAL_TIM_GET_COUNTER(htim);                             // 获取计数值，并转 int32_t 类型 此处AI报错 但每次采样计数值很小 不会溢出
    __HAL_TIM_SET_COUNTER(htim, 32767);                                                // 清空定时器的计数值，以便下一次采样
    enc->rpm = ((32767 - enc->raw_count) * 60) / (ENCODER_RESOLUTION * ENCODER_CYCLE); // 脉冲数*60s/(每转脉冲数×采样周期秒) 采样周期
    enc->filtered_rpm = 0.2f * enc->filtered_rpm + 0.8f * enc->rpm;                    // 对计算得到的转速进行低通滤波处理，减少噪声干扰 0.2
}

void Task_solver() // 放主函数while循环中 然后执行Task的任务 任务内的运动数据处理 就还在中断中进行 然后回来
{
    Task_Flag = __HAL_TIM_GET_COUNTER(&hlptim2);
    if (Task_Flag == Task_Count == 1)
    {
        Buzzer(); // 切换模式响一次
        Task1();
        Task_Count++;
    }
    if (Task_Flag == Task_Count == 2)
    {
        Buzzer(); // 切换模式响一次
        Task2();
        Task_Count++;
    }

    // 调试的时候 编码器拧3下执行3模式 4下执行4模式

    if (Task_Flag == Task_Count == 3 || Task_Flag == Task_Count == 4)
    {
        Buzzer(); // 切换模式响一次
        Task3();
        Task_Count++;
    }
}
