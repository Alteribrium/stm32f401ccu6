#ifndef SysClock_H
#define SysClock_H

#include <stm32f401xc.h>
#include <stdint.h>

void SysClock_init(void);
void SysTick_Handler(void);
void SysClock_delay(uint64_t milliseconds);

extern uint64_t msCounter;

#endif
