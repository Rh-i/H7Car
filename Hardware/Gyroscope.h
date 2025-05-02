#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H

#include "stm32h7xx_hal.h"
#include "usart.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "dma.h"

#define UART_Port GPIOA     // 串口号
#define UART_TX GPIO_PIN_12 // 发送 -> 陀螺仪
#define UART_RX GPIO_PIN_11 // 接收 <- 陀螺仪

// 数据结构封装
typedef struct
{
  float aX, aY, aZ;       // 加速度 (m/s²)
  float wX, wY, wZ;       // 角速度 (°/s)
  float Roll, Pitch, Yaw; // 欧拉角 (°)
} GyroscopeData_t;

// 函数声明
void Gyroscope_Init(void); //初始化
bool checkSum(uint8_t *data, size_t len);

// 全局变量
extern GyroscopeData_t gyroData;

#endif // _GYROSCOPE_H