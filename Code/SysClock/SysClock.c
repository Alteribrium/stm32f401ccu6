#include "SysClock.h"

void SysClock_init(void);
void SysTick_Handler(void);
void SysClock_delay(uint64_t milliseconds);

uint64_t msCounter;


void SysClock_init(void){
	msCounter = 0;
	SysTick->LOAD = 16000;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
	msCounter++;
}

void SysClock_delay(uint64_t milliseconds) {
	uint64_t startTime = msCounter;
	while(msCounter - startTime < milliseconds){
		__NOP();
	}
}
