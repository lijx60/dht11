#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

typedef enum Led_t{
	LED_R = GPIO_Pin_5,
	LED_Y = GPIO_Pin_6,
	LED_G = GPIO_Pin_7
} LED_t;

void Led_Init(void);
void Led_Breath(uint16_t GPIO_Pin, uint32_t time);

#endif
