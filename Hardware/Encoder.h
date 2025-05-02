#ifndef __ENCODER_H
#define __ENCODER_H

// 编码器相关测量 修改定时器句柄来采样
#define ENCODER_RESOLUTION 1320 // 编码器的分辨率，单位为脉冲数
#define ENCODER_CYCLE 1e-3      // 编码器的采样周期，单位为S 根据函数调用的周期修改 中断程序间隔时间

#include "tim.h"
#include "stm32h7xx_hal.h"
#include "math.h"
#include <stdlib.h>
#include "lptim.h"
#include "Task.h"

typedef struct
{
  int32_t raw_count;  // 编码器的原始计数值
  float rpm;          // 编码器的测定角速度，单位为转/分
  float filtered_rpm; // 经过低通滤波处理后的角速度 单位是转/分 w×R=角速度×轮子半径=线速度 进而求出路程速度 左右轮速度之和除以2即为车的速度
} Encoder;

extern Encoder encoder_L1,encoder_L2; // 自行根据自己的编码器来更改命名
extern Encoder encoder_R1,encoder_R2; // 自行根据自己的编码器来更改命名
extern volatile uint8_t Task_Flag;
extern volatile uint8_t Task_Count;

void Encoder_Init();
void Encoder_Update(Encoder *enc, TIM_HandleTypeDef *htim);
void Task_solver();

#endif // ENCODER_H