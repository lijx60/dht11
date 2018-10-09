#include "hc_sr04.h"
#include "delay.h"
#include "timer_interrupt.h"

#define HC_SR04_ECHO_PIN 	GPIO_Pin_6
#define HC_SR04_TRIG_PIN 	GPIO_Pin_7
#define HC_SR04_GPIO 		GPIOA
#define HC_SR04_Periph		RCC_APB2Periph_GPIOA

void HC_SR04_Init(void)
{
	RCC_APB2PeriphClockCmd(HC_SR04_Periph, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = HC_SR04_TRIG_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC_SR04_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = HC_SR04_ECHO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC_SR04_GPIO, &GPIO_InitStruct);
	
	GPIO_ResetBits(HC_SR04_GPIO, HC_SR04_TRIG_PIN);
}

uint32_t HC_SR04_GetDistance(void)
{
	GPIO_SetBits(HC_SR04_GPIO, HC_SR04_TRIG_PIN);
	Delay_us(10);
	GPIO_ResetBits(HC_SR04_GPIO, HC_SR04_TRIG_PIN);

	while (GPIO_ReadInputDataBit(HC_SR04_GPIO, HC_SR04_ECHO_PIN) == RESET);
	TIM_Cmd(TIM4, ENABLE);
	while (GPIO_ReadInputDataBit(HC_SR04_GPIO, HC_SR04_ECHO_PIN) == SET);
	TIM_Cmd(TIM4, DISABLE); 
	
	uint32_t HC_SR04_DATA = (TIM4_CNT * 1000 + TIM_GetCounter(TIM4)) / 58;
	TIM4_CNT = 0;
	TIM_SetCounter(TIM4, 0);
	
	return HC_SR04_DATA;
}
