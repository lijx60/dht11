#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#include "stm32f10x.h"

void HC_SR04_Init(void);
uint32_t HC_SR04_GetDistance(void);

#endif
