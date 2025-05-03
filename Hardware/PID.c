#include "PID.h"

// 单纯pid计算
// 调试确定最大积分累计值 MAX_OUTPUT
// 以及运行处理pid计算的函数 放在这里 减少代码重复度

const uint8_t SPEED_LOOP = 0;
const uint8_t DIRECTION_LOOP = 1;

DualPID ctrl = {
    .Kp_speed = 1.0, .Ki_speed = 0.0, .Kd_speed = 0.1, .Kp_direction = 0.5, .Ki_direction = 0.0, .Kd_direction = 0.05, // 示例值，根据实际情况调整
    .prev_error = {0, 0}                                                                                               // 确保所有元素都被初始化
};

float Dual_PID_Calculate(DualPID *pid, float target, float current, uint8_t loop_type, float dt)
{
  static float integral[2] = {0, 0}; // 积分项
  float error = target - current;
  float output = 0;

  // 内环（速度环）
  // 查阅资料得 速度环的计算需要得到RPM的值和PWM进行转换
  // RPM范围 0-80 PWM范围0-800 需要简单的转换就可表示RPM  速度环通过RPM来算 最终结果 *10 or /10 转换就可以了 默认值pwm设置500
  if (loop_type == SPEED_LOOP)
  {
    integral[0] += error * dt;
    output = pid->Kp_speed * error + pid->Ki_speed * integral[0] + pid->Kd_speed * (error - pid->prev_error[0]) / dt;
    pid->prev_error[0] = error;
  }

  // 循迹环
  if (loop_type == DIRECTION_LOOP)
  // output的范围为-125~125 +-到对应的pwm上
  {
    integral[1] += error * dt;
    output = pid->Kp_direction * error + pid->Ki_direction * integral[1] + pid->Kd_direction * (error - pid->prev_error[1]) / dt;
    pid->prev_error[1] = error;
  }

  // 防止积分饱和（可选）
  if (output > MAX_OUTPUT)
    output = MAX_OUTPUT;
  if (output < MIN_OUTPUT)
    output = MIN_OUTPUT;

  return output;
}

// PID计算的一个过程函数
// 除以2是取平均 乘以10是把RPM转换成pwm
int forward_left_pwm()
{
  return Dual_PID_Calculate(&ctrl, BASE_RPM + 20, encoder_L1.filtered_rpm, SPEED_LOOP, ENCODER_CYCLE)  * 10;
}
int forward_right_pwm()
{
  return Dual_PID_Calculate(&ctrl, BASE_RPM + 20, encoder_R1.filtered_rpm, SPEED_LOOP, ENCODER_CYCLE)* 10;
}

int sensor_left_pwm(int Sensor_pwm)
{
  return  Dual_PID_Calculate(&ctrl, BASE_RPM - Sensor_pwm / 10, encoder_L1.filtered_rpm, SPEED_LOOP, ENCODER_CYCLE)  * 10;
}

int sensor_right_pwm(int Sensor_pwm)
{
  return Dual_PID_Calculate(&ctrl, BASE_RPM + Sensor_pwm / 10, encoder_R1.filtered_rpm, SPEED_LOOP, ENCODER_CYCLE) * 10;
}