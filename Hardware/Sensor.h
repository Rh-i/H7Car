#ifndef _SENSOR_H
#define _SENSOR_H

#include "stm32h7xx_hal.h"

#define L2 GPIO_PIN_2 //PF2
#define L1 GPIO_PIN_3 //PF3
#define C  GPIO_PIN_4 //PF4
#define R1 GPIO_PIN_5 //PF5
#define R2 GPIO_PIN_6 //PF6

typedef struct
{
  uint8_t l2;
  uint8_t l1;
  uint8_t c;
  uint8_t r1;
  uint8_t r2;
}Sensors;

extern Sensors sensor;
extern volatile int Sensor_output; 

void Sensor_Dispose();

#endif // _SENSOR_H