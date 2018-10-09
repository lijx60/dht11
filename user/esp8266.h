#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f10x.h"
#include "usart.h"

#define ESP8266_BAUDRATE 115200
#define ESP8266_USART USART2

#define ESP8266_GetRecvFlag 		USART2_GetRecvFlag
#define ESP8266_ClearRecvFlag()		USART2_ClearRecvFlag()
#define ESP8266_RcvdBuf()			USART2_RcvdBuf()

void ESP8266_Init(void);
void ESP8266_CH_PD_PIN_Init(void);
void ESP8266_RecvMsg(void);

#endif
