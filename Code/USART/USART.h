#ifndef USART_H_
#define USART_H_
#include "../main.h"

void USART2_init(void);
void USART2_IRQHandler(void);
void USART2_DMA_init(void);
void USART2_DMA_SendData(uint8_t *data, uint32_t length);
void USART2_DMA_SendString(char *str);
uint32_t USART2_DMA_GetReceivedData(uint8_t *buffer, uint32_t max_len);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void USART2_Process_Received_Data(void);
void USART2_DMA_SendChar(char str);
extern char temp;

#endif
