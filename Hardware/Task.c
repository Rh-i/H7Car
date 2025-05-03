#include "Task.h"

/*
  主函数部分会一直在这里面执行 所以所有数据接收处理操作都必须在中断中实现
  这里面使用状态机实现的Task1 2 3 的控制 更加清晰

  中断调用情况
  TIM6计数完成中断: 1ms延迟
    计算电机实时RPM数值 读取:encoder_L1.filtered_rpm
    读取循迹数据:Sensor_output偏向数据 左读+右读-   sensor.l2 对应循迹点情况 1为黑
  DMA完成触发中断
    读取陀螺仪数据：gyroData.Yaw 此为航向角数据

  初始化时：小车放置于A点 车头垂直对着B点 初始状态循迹读不到黑线
*/

MotorState TaskState2 = MOVE_FORWARD1;
MotorState TaskState34 = MOVE_RIGHT1;

// 数据处理逻辑在 stm32h7xx_it.c

/* ************************************************************************* */

void Task1()
{
  // 左右同用pid计算行驶速度 读不到线时执行直走操作 读到线了就停止
  while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
  {
    Motor_Control(Task_left_pwm, Task_right_pwm);
  }

  Motor_Control(0, 0);
}

/* ************************************************************************* */

void Task2()
{
  switch (TaskState2)
  {
  case MOVE_FORWARD1: // 第一次直行不需要校正
    while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
    {
      Motor_Control(Task_left_pwm, Task_right_pwm);
    }
    TaskState2 = MOVE_SENSOR;
    break;

  case MOVE_SENSOR:

    // 这个循环要确保在0.2s内有循迹探头读取到值才会认为他在进行循迹（如果没有则不行）

    // 同样的也可以加入角度的判断来大致判断是否在循迹
    // 自动变成MOVE_FORWARD2 和 STOP
    Motor_Control(Task_left_pwm, Task_right_pwm);
    break;

  case MOVE_FORWARD2:
    // 需要先写校准代码 然后再写直走
    /*
      此处为方向校准代码 把校准的延时也加入进来 这个纯写死的东西 我就不写了
      可能因为循迹稳定性的原因 会导致每次出这里的角度都不同 那这个你想想办法 我不知道咋解
      Motor_Control(1,2) //写死的数
      Delay_ms(300)      //写死的数

      你也可以用陀螺仪的 detla角 来转向
      如果陀螺仪不偏移误差 那这个位置可以写转向环PID让航向角归零
    */
    while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
    {
      Motor_Control(Task_left_pwm, Task_right_pwm);
    }
    TaskState2 = MOVE_SENSOR;
    break;

  case STOP:
    Motor_Control(0, 0);
    break;


  }
}

/* ************************************************************************* */

void Task3()
{
  switch (TaskState34)
  {
  case MOVE_RIGHT1:

    // 方向校正
  
    // 开始直行 然后循迹
    while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
    {
      Motor_Control(Task_left_pwm, Task_right_pwm);
    }
    TaskState34 = MOVE_SENSOR;
    break;
  
  case MOVE_SENSOR:
    
    Motor_Control(Task_left_pwm, Task_right_pwm);
    break;

  case MOVE_LEFT2:

    // 方向校正 同上

    while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
    {
      Motor_Control(Task_left_pwm, Task_right_pwm);
    }
    TaskState34 = MOVE_SENSOR;
    break;

  case MOVE_RIGHT2:

    // 方向校正 同上

    while (sensor.c == 0 && sensor.l1 == 0 && sensor.l2 == 0 && sensor.r1 == 0 && sensor.r2 == 0)
    {
      Motor_Control(Task_left_pwm, Task_right_pwm);
    }
    TaskState34 = MOVE_SENSOR;
    break;

  }
}
