#ifndef Motor_h
#define Motor_h

// PC0 -> AIN1 | PC1 -> AIN2
// PC2 -> BIN1 | PC3 -> BIN2
// GPIOC没用宏定义

#include "main.h"
#include <stdlib.h>
#include <math.h>
#include "PID.h"
#include "stm32h7xx_hal.h"
#include "Encoder.h"

typedef struct Control
{
  int16_t left_pwm;
  int16_t right_pwm;
  float path_error;
  float delta_rpm;
  float left_target;
  float right_target;
} Control;

#define MOTOR_HTIM htim1 // 自己根据定义的计时器来更改

#define AIN_GPIO_Port GPIOD
#define BIN_GPIO_Port GPIOD

#define AIN1_Pin GPIO_PIN_0 // 修改正反转
#define AIN2_Pin GPIO_PIN_1
#define BIN1_Pin GPIO_PIN_2
#define BIN2_Pin GPIO_PIN_3

#define CIN_GPIO_Port GPIOD
#define DIN_GPIO_Port GPIOD

#define CIN1_Pin GPIO_PIN_4 // 修改正反转
#define CIN2_Pin GPIO_PIN_5
#define DIN1_Pin GPIO_PIN_6
#define DIN2_Pin GPIO_PIN_7

#define PWM_MAX 1000  // 最大PWM值
#define TRACK_VALUE 1 // 路径环控制量
#define SPEED_VALUE 2 // 速度环控制量

void Motor_Init();
void Motor_Control(int left_pwm, int right_pwm);

#endif // Motor_h