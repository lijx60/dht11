#include <string.h>
#include <stdio.h>
#include "esp8266.h"
#include "delay.h"
#include "led.h"
#include "dht11.h"
#include "hc_sr04.h"

#define ESP8266_CH_PD_PIN 	GPIO_Pin_4
#define ESP8266_CH_PD_GPIO	GPIOA

static uint8_t ESP8266_SendCmd(const char* cmd, char *ack, uint8_t wait_time);

void ESP8266_CH_PD_PIN_Init(void)
{
	RCC_APB2PeriphClockCmd(ESP8266_CH_PD_PIN, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = ESP8266_CH_PD_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ESP8266_CH_PD_GPIO, &GPIO_InitStruct);
	
	GPIO_SetBits(ESP8266_CH_PD_GPIO, ESP8266_CH_PD_PIN);
}

void ESP8266_Init(void)
{
	ESP8266_CH_PD_PIN_Init();
	
	Delay_ms(100);
	while (ESP8266_SendCmd("AT+CWSAP=\"ESP-Zhaoyg\",\"0413\",1,0", "OK", 10));
	while (ESP8266_SendCmd("AT+CWMODE=2", "OK", 5));
	while (ESP8266_SendCmd("AT+CIPMUX=1", "OK", 5));
	while (ESP8266_SendCmd("AT+CIPSERVER=1,8080", "OK", 5));
	
	USARTx_printf(USART3, "ESP8266 start succeed\r\n");
}

/* 成功返回0, 失败返回1 */
uint8_t ESP8266_SendCmd(const char* cmd, char *ack, uint8_t wait_time)
{
	if (ack == NULL || wait_time == 0)
	{
		return 1;
	}
	
	ESP8266_ClearRecvFlag();
	USARTx_printf(ESP8266_USART, "%s\r\n", cmd);
	
	while (--wait_time)
	{
		Delay_ms(20);
		if (ESP8266_GetRecvFlag() && strstr(ESP8266_RcvdBuf(), ack))
		{
			ESP8266_ClearRecvFlag();
			return 0;
		}
	}
	ESP8266_ClearRecvFlag();
	return 1;
}

void ESP8266_RecvMsg(void)
{
	if (ESP8266_GetRecvFlag())
	{
		//USARTx_printf(USART3, "%s", ESP8266_RcvdBuf());
		Delay_ms(10);
		if (strstr(ESP8266_RcvdBuf(), ",CONNECT"))
		{
			ESP8266_ClearRecvFlag();
			return;
		}
		char *p = strstr(ESP8266_RcvdBuf(), "+IPD");
		if (p != NULL)
		{
			/*if (strcmp(p + 9, "ledon") == 0)
			{
				GPIO_SetBits(GPIOA, LED_R);		
				while (ESP8266_SendCmd("AT+CIPSEND=0,6", "OK", 10));	
				while (ESP8266_SendCmd("ledon", "OK", 10));
			}
			else if (strcmp(p + 9, "ledoff") == 0)
			{
				GPIO_ResetBits(GPIOA, LED_R);
				while (ESP8266_SendCmd("AT+CIPSEND=0,7", "OK", 10));	
				while (ESP8266_SendCmd("ledoff", "OK", 10));
			}
			else*/ if (strcmp(p + 9, "dht11") == 0)
			{
				DHT11_Start();
				char str[30] = {0};
				if (DHT11_Check())
				{
					sprintf(str, "T: %02d H: %02d%%", 
						DHT11_GetTemperature(), DHT11_GetHumidity());
					while (ESP8266_SendCmd("AT+CIPSEND=0,12", "OK", 10));	
					while (ESP8266_SendCmd(str, "OK", 10));
				}
				else
				{
					while (ESP8266_SendCmd("AT+CIPSEND=0,12", "OK", 10));	
					while (ESP8266_SendCmd("dht11 error", "OK", 10));
				}
			}
			else if (strcmp(p + 9, "hcsr04") == 0)
			{
				char str[30] = {0};
				sprintf(str, "%4dcm", HC_SR04_GetDistance());
				while (ESP8266_SendCmd("AT+CIPSEND=0,7", "OK", 10));	
				while (ESP8266_SendCmd(str, "OK", 10));
			}

			
			ESP8266_ClearRecvFlag();
		}
	}
}
