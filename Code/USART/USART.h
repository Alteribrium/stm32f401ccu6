#ifndef USART_H_
#define USART_H_
#include "../main.h"

void USART2_init(void);
void USART2_IRQHandler(void);
void USART2_SendChar(uint8_t ch);
void USART2_SendString(char *str);

extern char temp;

#endif
