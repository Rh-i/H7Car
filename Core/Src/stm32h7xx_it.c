/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile long long int Task_left_pwm;
volatile long long int Task_right_pwm;

volatile uint32_t Temp_Time = 0;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles DMA1 stream0 global interrupt.
 */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
 * @brief This function handles DMA1 stream7 global interrupt.
 */
void DMA1_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream7_IRQn 0 */

  /* USER CODE END DMA1_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream7_IRQn 1 */

  /* USER CODE END DMA1_Stream7_IRQn 1 */
}

/**
 * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
 */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
  Encoder_Update(&encoder_L1, &htim3);
  // Encoder_Update(&encoder_L2, &htim4);
  Encoder_Update(&encoder_R1, &htim5);
  // Encoder_Update(&encoder_R2, &htim8);
  static uint32_t time_tim6;
  time_tim6 = Get_Micros();

  if (Buzzer_flag == 1)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    if (time_tim6 - Buzzer_time > 1e6)
    {
      Buzzer_flag = 0;
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    }
  }

  /* ************************************************************************* */

  if (Task_Flag == Task_Count == 1) // 任务1数据处理
  {
    Task_left_pwm = forward_left_pwm();
    Task_right_pwm = forward_right_pwm();
  }
  /* ************************************************************************* */

  if (Task_Flag == Task_Count == 2) // 任务2数据处理
  {
    if (TaskState2 == MOVE_FORWARD1) // 直行1
    {
      Task_left_pwm = forward_left_pwm();
      Task_right_pwm = forward_right_pwm();
    }

    if (TaskState2 == MOVE_SENSOR) // 循迹 两次均用这个
    {
      Sensor_Dispose(); // 实时计算偏差
      if (sensor.c || sensor.l1 || sensor.l2 || sensor.r1 || sensor.r2)
      {
        static int Sensor_pwm;
        Sensor_pwm = Dual_PID_Calculate(&ctrl, 0, Sensor_output, DIRECTION_LOOP, ENCODER_CYCLE);
        Task_left_pwm = sensor_left_pwm(Sensor_pwm);
        Task_right_pwm = sensor_right_pwm(Sensor_pwm);
      }
      else
      {
        if (Get_Micros() - Temp_Time > 2e5 && Temp_Time != 0) // 如果0.2s没识别到线 进入下一个模式 temp未更新的数据跳过
        {
          static uint8_t task2count = 0;
          task2count++;
          // 循迹了两次 第一次结束进入2 第二次结束停止
          if (task2count == 2)
          {
            TaskState2 = STOP;
            Temp_Time = 0;
            task2count = 0;
          }
          else
          {
            TaskState2 = MOVE_FORWARD2;
            Temp_Time = 0;
          }
        }
        Temp_Time = Get_Micros(); // 记录
      }
    }

    if (TaskState2 == MOVE_FORWARD2) // 直行2
    {
      Task_left_pwm = forward_left_pwm();
      Task_right_pwm = forward_right_pwm();
    }
  }

  /* ************************************************************************* */

  if (Task_Flag == Task_Count == 3) // 任务34的数据处理
  {
    if (TaskState34 == MOVE_RIGHT1 || TaskState34 == MOVE_RIGHT2 || TaskState34 == MOVE_LEFT2)
    {
      Task_left_pwm = forward_left_pwm();
      Task_right_pwm = forward_right_pwm();
    }

    if (TaskState34 == MOVE_SENSOR) // 循迹 八次均用这个
    {
      Sensor_Dispose(); // 实时计算偏差
      if (sensor.c || sensor.l1 || sensor.l2 || sensor.r1 || sensor.r2)
      {
        static int Sensor_pwm34;
        Sensor_pwm34 = Dual_PID_Calculate(&ctrl, 0, Sensor_output, DIRECTION_LOOP, ENCODER_CYCLE);
        Task_left_pwm = sensor_left_pwm(Sensor_pwm34);
        Task_right_pwm = sensor_right_pwm(Sensor_pwm34);
      }
      else
      {
        if (Get_Micros() - Temp_Time > 2e5 && Temp_Time != 0) // 如果0.2s没识别到线 进入下一个模式 temp未更新的数据跳过
        {
          static uint8_t task34count = 0;
          task34count++;
          // 循迹了8次 第一次结束进入左 第二次结束进入右
          if (task34count == 8 && Task_Flag == 4 || task34count == 2 && Task_Flag == 3)
          {
            TaskState34 = STOP;
            Temp_Time = 0;
            task34count = 0;
          }
          else if (task34count % 2 == 1)
          {
            TaskState34 = MOVE_LEFT2;
            Temp_Time = 0;
          }
          else if (task34count % 2 == 0)
          {
            TaskState34 = MOVE_RIGHT2;
            Temp_Time = 0;
          }
        }
        Temp_Time = Get_Micros(); // 记录
      }
    }
  }

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
