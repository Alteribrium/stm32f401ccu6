#ifndef BUTTON_PB6_H_
#define BUTTON_PB6_H_

#include "main.h"

void BUTTON_PB6_Init(void);
void EXTI9_5_IRQHandler(void);
void TIM2_IRQHandler(void);

extern uint8_t PB6_state;

#endif
