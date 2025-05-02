#include "Delay.h"

// 确保定时器配置为32位自由运行模式（关键）（71分钟）
// 外部时钟的执行 是独立于cpu的 所以延迟是外部参考系来看绝对精准 而不是经过中断等等的调用后精准
// 如果定时器cnt是65535的只需要在定时器完成触发中断里面让那个值+1 然后取模运算得出值 可以得到超长延时。
// 这里面的通过软件层面好像也实现了处理溢出的情况
// 无参数需要修改 只需要注意定时器的cnt上限

uint32_t Get_Micros()
{
  return (uint32_t)__HAL_TIM_GET_COUNTER(&DELAY_HTIM);
}

// 新增：支持超长延时的分段处理
void Delay_us(uint32_t us)
{
  while (us > 0)
  {
    // 修改1：分段最大值为0xFFFFFFFF-1，避免目标时间溢出
    uint32_t chunk = (us > (0xFFFFFFFF - 1)) ? (0xFFFFFFFF - 1) : us;
    uint32_t start_time = Get_Micros();

    // 处理溢出场景
    while (1)
    {
      uint32_t current_time = Get_Micros();
      uint32_t elapsed;
      if (current_time >= start_time)
      {
        elapsed = current_time - start_time;
      }
      else
      {
        // 修改2：计算绕过一圈的经过时间
        elapsed = current_time + (0xFFFFFFFF - start_time + 1);
      }
      if (elapsed >= chunk)
      {
        break;
      }
      __NOP();
    }
    us -= chunk; // 减去已完成的延时
  }
}

// 优化后的Delay_ms（参数溢出保护）
void Delay_ms(uint32_t ms)
{
  // 计算最大安全值：UINT32_MAX / 1000 = 4294967295 / 1000 = 4294967.295
  if (ms > 4294967295 / 1000)
  {
    while (1)
      ; // 参数过大时进入死循环（或根据需求处理）
  }
  Delay_us(ms * 1000); // 自动处理分段
}
