#include "Gyroscope.h"

// 驱动我大概写了 并没有测试读取位置设置 你测试的时候根据我发的数据手册 然后问ai修改这里面的数据处理内容 只会有小错误 可以串口打印到电脑上再修改
// 还有需要电脑对这个陀螺仪校准 不清楚误差问题 可能会很大
// 最终数据在struct里面 不需要调用任何函数 只需要初始化 就可以读取结构体数据来访问了 没有计算函数是需要运行的（DMA+触发中断自动进行 回调函数也在这里面 如果有问题就去看DMA中断函数）

GyroscopeData_t gyroData;
static uint8_t RxBuffer[52]; // 扩展缓冲区支持多数据包（13字节×4）

// 配置DMA接收
void Gyroscope_Init()
{
  HAL_UART_Receive_DMA(&huart4, RxBuffer, sizeof(RxBuffer));
}

// 数据解析辅助函数
static bool ValidateData(char *data)
{
  // 根据实际数据结构调整验证逻辑（示例）
  if (abs((int16_t)((data[3] << 8) | data[2])) > 32767)
    return false;
  if (abs((int16_t)((data[5] << 8) | data[4])) > 32767)
    return false;
  if (abs((int16_t)((data[7] << 8) | data[6])) > 32767)
    return false;
  return true;
}

// 计算函数（小端存储处理）
static float CalculateAcceleration(uint8_t highByte, uint8_t lowByte)
{
  int16_t raw = (highByte << 8) | lowByte;
  return (float)raw / 32768.0f * 16.0f; // 单位g
}

static float CalculateAngularVelocity(uint8_t highByte, uint8_t lowByte)
{
  int16_t raw = (highByte << 8) | lowByte;
  return (float)raw / 32768.0f * 2000.0f; // 单位°/s
}

static float CalculateEulerAngle(uint8_t highByte, uint8_t lowByte)
{
  int16_t raw = (highByte << 8) | lowByte;
  return (float)raw / 32768.0f * 360.0f; // 单位°
}

// 校验和函数
bool checkSum(uint8_t *data, size_t len)
{
  uint8_t sum = 0;
  for (size_t i = 0; i < len - 1; i++)
  {
    sum += data[i];
  }
  return (sum == data[len - 1]);
}

// DMA接收完成回调（循环模式自动触发）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart != &huart4)
    return;

  // 遍历缓冲区查找有效数据包
  uint8_t *p = RxBuffer;
  while (p <= RxBuffer + sizeof(RxBuffer) - 2) // 确保剩余空间足够检测0x55
  {
    if (*p == 0x55 && (p[1] == 0x51 || p[1] == 0x52 || p[1] == 0x53))
    {
      char tempBuffer[13]; // JY61P数据包固定长度13字节
      memcpy(tempBuffer, p, 13);

      if (checkSum((uint8_t *)tempBuffer, 13) && ValidateData(tempBuffer))
      {
        switch (tempBuffer[1])
        {
        case 0x51:                                                           // 加速度数据
          gyroData.aX = CalculateAcceleration(tempBuffer[3], tempBuffer[2]); // 高字节在前
          gyroData.aY = CalculateAcceleration(tempBuffer[5], tempBuffer[4]);
          gyroData.aZ = CalculateAcceleration(tempBuffer[7], tempBuffer[6]);
          break;
        case 0x52:                                                              // 角速度数据
          gyroData.wX = CalculateAngularVelocity(tempBuffer[3], tempBuffer[2]); // 高字节在前
          gyroData.wY = CalculateAngularVelocity(tempBuffer[5], tempBuffer[4]);
          gyroData.wZ = CalculateAngularVelocity(tempBuffer[7], tempBuffer[6]);
          break;
        case 0x53:                                                           // 欧拉角数据
          gyroData.Roll = CalculateEulerAngle(tempBuffer[3], tempBuffer[2]); // 高字节在前
          gyroData.Pitch = CalculateEulerAngle(tempBuffer[5], tempBuffer[4]);
          gyroData.Yaw = CalculateEulerAngle(tempBuffer[7], tempBuffer[6]);
          break;
        }
      }
      p += 13; // 跳过已处理的数据包
    }
    else
    {
      p++; // 移动指针继续查找
    }
  }
}