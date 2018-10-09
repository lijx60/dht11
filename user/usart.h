#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

/* 缓存大小 */
#define USART1_RX_BUF_LEN    128
#define USART2_RX_BUF_LEN    128
#define USART3_RX_BUF_LEN    128

/* 波特率 */
#define USART1_BAUDRATE      9600
#define USART2_BAUDRATE      115200
#define USART3_BAUDRATE      115200

/* 数据位和校验位 */
#define FORMAT_8none         0x00
#define FORMAT_8even         0x01
#define FORMAT_8odd          0x02

/* stdio重定向 */
#define IO_REDIRECTION 		 (USART3)

/* 串口初始化 */
void USART_AllInit(void);
void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void Stm32_Uartx_Init(USART_TypeDef* USARTx, uint32_t Baudrate, uint16_t Format);

/* 串口输出函数 */
void USARTx_printf(USART_TypeDef* USARTx, const char *Data, ...);
void usart_send_str(USART_TypeDef* USARTx, const char *str);

/* 获取接受状态 */
uint8_t USART1_GetRecvFlag(void);
uint8_t USART2_GetRecvFlag(void);
uint8_t USART3_GetRecvFlag(void);

/* 清除接受状态 */
void USART1_ClearRecvFlag(void);
void USART2_ClearRecvFlag(void);
void USART3_ClearRecvFlag(void);

/* 获取接受的消息 */
const char *USART1_RcvdBuf(void);
const char *USART2_RcvdBuf(void);
const char *USART3_RcvdBuf(void);

#endif

