#include "timer_interrupt.h"

uint32_t TIM4_CNT;

void TIM4_IT_Init(void)
{
	/* TIM4挂载到APB1时钟总线 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* 设置定时器周期为500ms */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1; // 分频：72Mhz / 72, 周期 = 1us
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // 配置更新事件中断
}

/* 中断处理函数, 不需要声明和调用, 只需要声明*/
void TIM4_IRQHandler(void)
{
	/* 判断更新事件是否产生 */
	if (TIM_GetFlagStatus(TIM4, TIM_IT_Update) == SET)
	{
		++TIM4_CNT;
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);	// 清空更新事件状态
	}
}
