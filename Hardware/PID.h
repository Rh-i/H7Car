#ifndef _PID_H
#define _PID_H

#include "tim.h"
#include "stm32h7xx_hal.h"
#include "Task.h"

#define MAX_OUTPUT (-10.0f)
#define MIN_OUTPUT (10.0f)

typedef struct DualPID
{
  // 内环（速度控制）
  float Kp_speed, Ki_speed, Kd_speed;
  // 方向环
  float Kp_direction, Ki_direction, Kd_direction;
  // 站立环
  float prev_error[2]; // 每个环的前一次误差
} DualPID;

extern DualPID ctrl;

extern const uint8_t SPEED_LOOP;
extern const uint8_t DIRECTION_LOOP;

float Dual_PID_Calculate(DualPID *pid, float target, float current, uint8_t loop_type, float dt);

int forward_left_pwm();
int forward_right_pwm();
int sensor_left_pwm(int Sensor_pwm);
int sensor_right_pwm(int Sensor_pwm);

#endif // _PID_H