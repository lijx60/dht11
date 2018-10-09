#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);

void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);
void Delay_s(uint16_t ns);

#endif

