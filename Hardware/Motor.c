#include "Motor.h"

// 用RPM代表速度 RPM是角速度好像 你去看看encoder里面的驱动咋写的 最后转为PWM输出
// 先路径环 再速度环 后转向环 最后直立环 要不可以改改位置也行 等等
// 我配置时钟配置到2000hz吧 然后中断1ms调用一次编码器以及pwm驱动我觉得也行 不行你自己改
// 13左轮子 24右轮子  AB为前 CD为后  A左B右  如图
// 1A    2B
//
// 3C    4D

void Motor_Init()
{
  HAL_TIM_PWM_Start(&MOTOR_HTIM, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&MOTOR_HTIM, TIM_CHANNEL_2);
  // HAL_TIM_PWM_Start(&MOTOR_HTIM, TIM_CHANNEL_3);
  // HAL_TIM_PWM_Start(&MOTOR_HTIM, TIM_CHANNEL_4);
}

void Motor_Control(int left_pwm, int right_pwm) // 最终处理正反转 等等 控制pwm PWM计算我不写在这了 单独算
{
  // 正反转控制 正转就是pwm正 反转就是负 正反转调整同时也要测试引脚数据 可以软件上颠倒 三元判断
  HAL_GPIO_WritePin(AIN_GPIO_Port, AIN1_Pin, (left_pwm > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AIN_GPIO_Port, AIN2_Pin, (left_pwm > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
  HAL_GPIO_WritePin(BIN_GPIO_Port, BIN1_Pin, (right_pwm > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BIN_GPIO_Port, BIN2_Pin, (right_pwm > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
  // HAL_GPIO_WritePin(CIN_GPIO_Port, CIN1_Pin, (left_pwm > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(CIN_GPIO_Port, CIN2_Pin, (left_pwm > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
  // HAL_GPIO_WritePin(DIN_GPIO_Port, DIN1_Pin, (right_pwm > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(DIN_GPIO_Port, DIN2_Pin, (right_pwm > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);

  // 限制PWM范围
  left_pwm = fmax(fmin(left_pwm, PWM_MAX), -PWM_MAX);
  right_pwm = fmax(fmin(right_pwm, PWM_MAX), -PWM_MAX);

  // 设置PWM输出 PWM最小值以及死区问题忽略不计 应该无所谓的
  __HAL_TIM_SET_COMPARE(&MOTOR_HTIM, TIM_CHANNEL_1, (uint16_t)(left_pwm > 0 ? left_pwm : -left_pwm));
  __HAL_TIM_SET_COMPARE(&MOTOR_HTIM, TIM_CHANNEL_2, (uint16_t)(right_pwm > 0 ? right_pwm : -right_pwm));
  // __HAL_TIM_SET_COMPARE(&MOTOR_HTIM, TIM_CHANNEL_3, (uint16_t)(left_pwm > 0 ? left_pwm : -left_pwm));
  // __HAL_TIM_SET_COMPARE(&MOTOR_HTIM, TIM_CHANNEL_4, (uint16_t)(right_pwm > 0 ? right_pwm : -right_pwm));
}
