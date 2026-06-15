#ifndef __USER_UART_H__
#define __USER_UART_H__

#include "ring_buffer.h"
#include "IfxAsclin_Asc.h" // 使用 iLLD 的 ASC 驱动

#define USART_RECV_BUF_SIZE 512
#define USART_SEND_BUF_SIZE 512

typedef struct
{
    IfxAsclin_Asc *ascHandle; // TC264 ASC 句柄指针
    RingBufferTypeDef *sendBuf;
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;

// 声明全局句柄
extern Usart_DataTypeDef FSUS_Usart;

void User_Uart_Init(IfxAsclin_Asc *ascHandle);
void Usart_SendAll(Usart_DataTypeDef *usart);

#endif
