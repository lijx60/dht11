#include "usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/**
  * @attention
  *
  *串口相关操作函数，完成串口的初始化操作，还定义了接收缓存，接收状态标志及接收长度信息
  *在做移植时，注意将相关变量移植到对应的文件中
  *
  ******************************************************************************
  */

/* 消息接收标志位 */
__IO uint8_t usart1_rcvd_flag;
__IO uint8_t usart2_rcvd_flag;
__IO uint8_t usart3_rcvd_flag;

/* 消息接收长度 */
__IO uint8_t usart1_rcvd_len = 0;
__IO uint8_t usart2_rcvd_len = 0;
__IO uint8_t usart3_rcvd_len = 0;

/* 消息接收数组 */
char usart1_rcvd_buf[USART1_RX_BUF_LEN];
char usart2_rcvd_buf[USART2_RX_BUF_LEN];
char usart3_rcvd_buf[USART3_RX_BUF_LEN];

/* 整形转字符，stm32不支持标准的itoa和atoi, 这里自己实现 */
static char *myitoa(int value, char *string, int radix);
//static int myatoi(const char *str);

void USART_AllInit(void)
{
	USART1_Init();
	USART2_Init();
	USART3_Init();
}

void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*
	*  USART1_TX -> PA9 , USART1_RX -> PA10
	*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	Stm32_Uartx_Init(USART1, USART1_BAUDRATE, FORMAT_8none);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/*
	*  USART2_TX -> PA2 , USART2_RX -> PA3
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	Stm32_Uartx_Init(USART2, USART2_BAUDRATE, FORMAT_8none);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}


void USART3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	/*
	*  USART3_TX -> PB10 , USART3_RX -> PB11
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	Stm32_Uartx_Init(USART3, USART3_BAUDRATE, FORMAT_8none);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


void Stm32_Uartx_Init(USART_TypeDef* USARTx, uint32_t Baudrate, uint16_t Format)
{
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = Baudrate;
	
	switch(Format)
	{
		default:
		case FORMAT_8none:
			USART_InitStruct.USART_WordLength = USART_WordLength_8b;
			USART_InitStruct.USART_Parity = USART_Parity_No;
			break;
		case FORMAT_8even:
			USART_InitStruct.USART_WordLength = USART_WordLength_9b;
			USART_InitStruct.USART_Parity = USART_Parity_Even;
			break;
		case FORMAT_8odd:
			USART_InitStruct.USART_WordLength = USART_WordLength_9b;
			USART_InitStruct.USART_Parity = USART_Parity_Odd;
			break;
	}

	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_Init(USARTx, &USART_InitStruct);
	
	USART_Cmd(USARTx, ENABLE);
	//USART_ClearFlag(USARTx, USART_FLAG_TC);
}


void usart_send_str(USART_TypeDef* USARTx, const char *str)
{
	USART_GetFlagStatus(USARTx, USART_FLAG_TC);
	while(*str !='\0')
	{
		USART_SendData(USARTx, *str);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
		{ }
		USART_ClearFlag(USARTx, USART_FLAG_TC);
		str++;
	}
}


//将输出重定向到USART3
int fputc(int ch, FILE *f)
{
	USART_SendData(IO_REDIRECTION, (char)ch);
	while (USART_GetFlagStatus(IO_REDIRECTION, USART_FLAG_TC) == RESET)
	{ }
	USART_ClearFlag(IO_REDIRECTION, USART_FLAG_TC);
	return (ch);
}	

//将输入重定向到USART3
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{ }
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	return (char)USART_ReceiveData(USART1);
}

/* 整形转字符 */
char *myitoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/* 字符串转整形*/
/*int myatoi(const char *str)
{
	int s=0;
	uint8_t falg=0;
	
	while(*str==' ')
	{
		str++;
	}

	if(*str=='-'||*str=='+')
	{
		if(*str=='-')
		falg=1;
		str++;
	}

	while(*str>='0'&&*str<='9')
	{
		s=s*10+*str-'0';
		str++;
		if(s<0)
		{
			s=2147483647;
			break;
		}
	}
	return s*(falg?-1:1);
}*/

/* 使用该函数实现串口的printf功能，注意是部分实现 */
void USARTx_printf(USART_TypeDef* USARTx, const char *Data,...)
{
	const char *str;
	int d;   
	char buf[16];
	va_list ap;
	va_start(ap, Data);
	while (*Data != 0)
	{	
		USART_GetFlagStatus(USARTx, USART_FLAG_TC);
		if (*Data == 0x5c)  //'\'
		{									  
            switch (*++Data)
            {
			case 'r':
				USART_SendData(USARTx,0x0d);
				Data++;
				break;

			case 'n':
				USART_SendData(USARTx,0x0a);	
				Data++;
				break;
                      
			default:
				Data++;
				break;
			}			 
      }
      else if (*Data == '%')
      {
			switch (*++Data)
			{				
				case 's':	
					str = va_arg(ap, const char *);
					for ( ; *str; str++) 
					{
						USART_SendData(USARTx, *str);
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
					}
					Data++;
					break;
				case 'd':	
					d = va_arg(ap, int);
					myitoa(d, buf, 10);
					for (str = buf; *str; str++) 
					{
						USART_SendData(USARTx, *str);
						while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
					}
					Data++;
					break;
				default:
					Data++;
					break;
			}		 
		} 
		else 
		{
			USART_SendData(USARTx, *Data++);
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		}
	}
}


uint8_t USART1_GetRecvFlag(void)
{
	return usart1_rcvd_flag;
}

uint8_t USART2_GetRecvFlag(void)
{
	return usart2_rcvd_flag;
}

uint8_t USART3_GetRecvFlag(void)
{
	return usart3_rcvd_flag;
}

void USART1_ClearRecvFlag(void)
{
	memset(usart1_rcvd_buf, 0, sizeof(usart1_rcvd_buf));
	usart1_rcvd_len = 0;
	usart1_rcvd_flag = 0;
}

void USART2_ClearRecvFlag(void)
{
	memset(usart2_rcvd_buf, 0, sizeof(usart2_rcvd_buf));
	usart2_rcvd_len = 0;
	usart2_rcvd_flag = 0;
}

void USART3_ClearRecvFlag(void)
{
	memset(usart3_rcvd_buf, 0, sizeof(usart3_rcvd_buf));
	usart3_rcvd_len = 0;
	usart3_rcvd_flag = 0;
}


const char *USART1_RcvdBuf(void)
{
	return usart1_rcvd_buf;
}

const char *USART2_RcvdBuf(void)
{
	return usart2_rcvd_buf;
}

const char *USART3_RcvdBuf(void)
{
	return usart3_rcvd_buf;
}

/** @brief  This function handles USART2 interrupt request. */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		usart1_rcvd_buf[usart1_rcvd_len++ % USART1_RX_BUF_LEN]
										= USART_ReceiveData(USART1);
		usart1_rcvd_flag = 1 ;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/** @brief  This function handles USART2 interrupt request. */
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		usart2_rcvd_buf[usart2_rcvd_len++ % USART2_RX_BUF_LEN] 
											= USART_ReceiveData(USART2);
		usart2_rcvd_flag = 1 ;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

/** @brief  This function handles USART2 interrupt request. */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		usart3_rcvd_buf[usart3_rcvd_len++ % USART3_RX_BUF_LEN] 
										= USART_ReceiveData(USART3);
		usart3_rcvd_flag = 1 ;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}


