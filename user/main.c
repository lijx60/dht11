#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#include "nvic.h"
#include "usart.h"
#include "delay.h"
#include "timer_interrupt.h"
#include "led.h"
#include "dht11.h"
#include "hc_sr04.h"
#include "esp8266.h"

int main()
{
	NVIC_Config();
	
	USART_AllInit();
	
	TIM4_IT_Init();
	Delay_Init();
	
	Led_Init();
	DHT11_Init();	// 温湿度传感器
	HC_SR04_Init();	// 超声波传感器
	ESP8266_Init(); // WiFi模块
	
	while(1)
	{	
		ESP8266_RecvMsg();
		/* MCU收到PC指令 */
//		if (USART3_GetRecvFlag() && strstr(USART3_RcvdBuf(), "\r\n"))
//		{	/* MCU转发指令到ESP8266 */
//			USARTx_printf(ESP8266_USART, "%s", USART3_RcvdBuf());
//			USART3_ClearRecvFlag();
//		}
//		/* MCU收到ESP8266回复 */
//		if (ESP8266_GetRecvFlag() && strstr(ESP8266_RcvdBuf(), "\r\n"))
//		{	/* MCU转发回复到PC */
//			USARTx_printf(USART3, "%s", ESP8266_RcvdBuf());
//			ESP8266_ClearRecvFlag();
//		}
		/* 超声波传感器
		printf("|HC_SR04| %dcm\r\n", HC_SR04_GetDistance());
		Delay_s(1); */
		
		/* 温湿度传感器
		DHT11_Start();
		if (DHT11_Check())
		{
			printf("|DHT11| Temperature: %dCelsius  Humidity: %d%%\r\n", 
			DHT11_GetTemperature(), DHT11_GetHumidity());
		}
		else
		{
			printf("|DHT11| Check error!\r\n");
		}
		Delay_s(3); */
		
		/*	串口通信
		if (USART3_GetRecvFlag() && strstr(USART3_RcvdBuf(), "\r\n"))
		{
			//USARTx_printf(USART3, "%s", USART3_RcvdBuf());
			printf("%s", USART3_RcvdBuf());
			
			USART3_ClearRecvFlag();
		} */
	}
}
