#ifndef USART1_H_
#define USART1_H_
#include "../main.h"

void USART1_init(void);
void USART1_DMA_SendString(char *str, uint16_t l);
void Modbus_Work(void);
#endif
