#ifndef BUTTON_PB10_H_
#define BUTTON_PB10_H_

#include "main.h"

void BUTTON_PB10_Init(void);
void EXTI9_5_IRQHandler(void);
void TIM2_IRQHandler(void);

extern uint8_t PB10_state;

#endif
