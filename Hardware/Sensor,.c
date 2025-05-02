#include "Sensor.h"

Sensors sensor;

volatile int Sensor_output = 0; 

// 循迹模块处理 往PID里面扔
// 计算转向环的数值 需要测量出检测到的角度 可能会有问题 输出正负都有 左-右+
// 我需要写它读不到线的时候需要执行那个脱线走的模式 所以可能循迹没调好他会识别错误 就在半圆处就漂移了


void Sensor_Dispose() 
{
  sensor.l2 = HAL_GPIO_ReadPin(GPIOE, L2);
  sensor.l1 = HAL_GPIO_ReadPin(GPIOE, L1);
  sensor.c = HAL_GPIO_ReadPin(GPIOE, C);
  sensor.r1 = HAL_GPIO_ReadPin(GPIOE, R1);
  sensor.r2 = HAL_GPIO_ReadPin(GPIOE, R2);

  // 左边传感器判断逻辑
  if (sensor.l2 == 1 && sensor.l1 == 0)
  {
    Sensor_output = 250; // 示例
  }
  else if (sensor.l1 == 1 && sensor.l2 == 1)
  {
    Sensor_output = 120;
  }
  else if (sensor.l1 == 1 && sensor.l2 == 0)
  {
    Sensor_output = 50;
  }

  if (sensor.c == 1)
  {
    // 中间传感器相关的逻辑
    Sensor_output = 0;
  }

  // 右边传感器判断逻辑
  if (sensor.r2 == 1 && sensor.r1 == 0)
  {
    Sensor_output = -250; // 取负
  }
  else if (sensor.r1 == 1 && sensor.r2 == 1)
  {
    Sensor_output = -120; // 取负
  }
  else if (sensor.r1 == 1 && sensor.r2 == 0)
  {
    Sensor_output = -50; // 取负
  }

}

