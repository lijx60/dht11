#ifndef __TIMER_INTERRUPT_H__
#define __TIMER_INTERRUPT_H__

#include "stm32f10x.h"

extern uint32_t TIM4_CNT;

extern void TIM4_IT_Init(void);
extern void NVIC_Config(void);

#endif
