#include "user_uart.h"

uint8_t usart1SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart1SendRingBuf;
RingBufferTypeDef usart1RecvRingBuf;
Usart_DataTypeDef FSUS_Usart;

void User_Uart_Init(IfxAsclin_Asc *ascHandle)
{
    RingBuffer_Init(&usart1SendRingBuf, USART_SEND_BUF_SIZE, usart1SendBuf);
    RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE, usart1RecvBuf);

    FSUS_Usart.recvBuf = &usart1RecvRingBuf;
    FSUS_Usart.sendBuf = &usart1SendRingBuf;
    FSUS_Usart.ascHandle = ascHandle;
}

// TC264 串口发送实现
void Usart_SendAll(Usart_DataTypeDef *usart)
{
    while (RingBuffer_GetByteUsed(usart->sendBuf))
    {
        uint8_t value = RingBuffer_Pop(usart->sendBuf);
        // 使用 iLLD 阻塞发送
        IfxAsclin_Asc_blockingWrite(usart->ascHandle, value);
    }
}

/*
 * 注意：TC264 的接收中断需要在工程的 ISR 文件中调用。
 * 你需要确保在串口接收中断中调用：
 * RingBuffer_Push(FSUS_Usart.recvBuf, receivedByte);
 */
