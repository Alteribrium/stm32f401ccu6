#ifndef SysClock_H_
#define SysClock_H_

#include "main.h"

void SysClock_init(void);
void SysTick_Handler(void);
void SysClock_delay(uint64_t milliseconds);

extern uint64_t msCounter;

#endif
