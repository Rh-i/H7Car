/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention      :2025.5.3 电赛选拔
 *
 * 0.未完成部分：
 *   调参
 *   蜂鸣器没弄 （你在Task函数内加 让他响一下就行）
 *   一定要看完所有的文件以及注释之后你再着手修改 别懂一半就改 容易出问题
 *   检测模式用旋钮编码器切换模式
 *   有啥问题和我反馈 别自己调了无用功
 *   中断函数和Task文件 俩个进行交互需要extern的 不要乱写 不会就问 报错了就跟我说 查ai也行
 *   所有电机的正反转没有去校正 你需要校正 陀螺仪也需要用串口的上位机校准 如何使用这个陀螺仪你来学(JY61P 型号) 我不想搞了
 *   循迹抽风了就再换一个 按照我这个接线 实在看不懂接线 那你总能看懂代码把 自己想办法改 CUBEMX都有哪个引脚 我没备注就是了
 *   OLED我没写它的使用 你要写你就开一个新的中断来更新屏幕 软件iic的 别搞其他花活 
 * 
 * 1. 所有全局变量的定义or声明都在各个.h文件中（包括main.h的几个变量）。各个结构体也是
 * 
 * 2. 代码存在位置：（每一个对应的头文件都有写代码）
 *    1. 陀螺仪的：Gyroscope.c （需要调试 以及校准到0）
 *    2. 循迹模块：Sensor.c   （循迹模块以及对应的权重output修改）
 *    3. 输出计算：PID.c      （PID计算 两环 没调参  以及各个运行模式的pwm计算函数）
 *    4. 编码器的：Encoder.c （电机编码器和旋钮编码器）
 *    5. 屏幕显示：OLED.c （软件IIC 计数1000刷新 写在while循环里）            
 *    6. 主函数的：main.c （这里面只写了类状态机的任务执行 .h内没文件）
 *    7. 自制时钟：Delay.c（32位 71分钟才能完成一次）
 *    8. 电机控制：Motor.c（电机正反转基础在这里写的） 
 *    9. 中断调用：stm32h7xx_it.c 中断触发函数 （最下面 好好找） （和Task执行函数相互照应）
 *    10.任务执行：Task.c  所有的执行（Task.c中是执行逻辑 中断中是数据接收处理逻辑 看代码的时候要俩一起看 一起改）
 * 
 * 3.统计外设使用情况（引脚自己去cube里面找 我在代码里面是能找到引脚咋使用的）
 *    1.陀螺仪 UART4 带了DMA以及终端数据接收以及处理
 *    2.Motor TIM1 以及八个控制正反转的gpio PD0-PD7
 *    3.Encoder TIM3 TIM4 TIM5 TIM8 各两个引脚   旋钮编码器LPTIM2（可能有bug）
 *    4.Delay TIM2 32位的 不会溢出
 *    5.其他就是gpio输入输出引脚
 * 
 * 4.中断使用情况 （TIM原生为275MHZ）（优先级没指定 都是0）
 *    1.陀螺仪处理的dma完成触发中断
 *    2.1ms中断进行编码器读数 pwm输出 （TIM6）
 * 
 * 
 * 
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "lptim.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Delay.h"
#include "Encoder.h"
#include "Gyroscope.h"
#include "OLED.h"
#include "PID.h"
#include "Sensor.h"
#include "Motor.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM6_Init();
  MX_LPTIM2_Init();
  /* USER CODE BEGIN 2 */
  Gyroscope_Init();
  Encoder_Init();
  Motor_Init();

  OLED_Init();
  HAL_Delay(100);
  OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // 所有的任务都在这个函数里面 所以while循环会一直在这个函数内部执行 所以除了这个函数别的什么都不要写进while循环 如果需要oled屏幕显示的话 需要自己配置定时器中断显示 20hz就行 
    // 这个代码需要旋钮编码器来选择模式 记得安装一个 任务执行只能1 2 3的顺序 通过旋转编码器选择模式  不能乱序执行 ||若要测试特定的就拧到对应位置测一次 然后断电|| 重复此前操作可以测试第二次 亦或者你直接把某个值写成对应的数据也可以 
    void Task_solver(); 

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 34;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 3072;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
