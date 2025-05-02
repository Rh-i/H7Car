#ifndef _Delay_h
#define _Delay_h

#define DELAY_HTIM htim8 // 修改定时器 通过预分频让定时器频率为1MHZ

#include "tim.h"
#include "stm32h7xx_hal.h"

uint32_t Get_Micros();
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);

#endif // _Delay_h