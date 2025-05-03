#ifndef _TASK_H
#define _TASK_H

#include "main.h"
#include "Delay.h"
#include "Encoder.h"
#include "Gyroscope.h"
#include "OLED.h"
#include "PID.h"
#include "Sensor.h"
#include "Motor.h"
#include "stm32h7xx_it.h"

// 最大值为800 RPM与PWM 1：10对应
#define BASE_RPM 50
#define BASE_PWM 500

typedef enum
{
  MOVE_FORWARD1, // 问题1 2 初始直行
  MOVE_FORWARD2, // 问题1 2 校准直行
  MOVE_SENSOR,   // ALL：   循迹处理
  MOVE_LEFT2,    // 问题3 4 左转
  MOVE_RIGHT2,   // 问题3 4 右转
  MOVE_RIGHT1,   // 问题3 4 右转 初始
  STOP,          // 停车
} MotorState;

void Task1();
void Task2();
void Task3();
void Buzzer();

extern MotorState TaskState2, TaskState34;
extern volatile uint8_t Buzzer_flag;
extern volatile uint32_t Buzzer_time;


#endif