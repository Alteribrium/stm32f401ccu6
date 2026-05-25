#include "PB121314.h"

void B121314_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		GPIOB->MODER &= ~GPIO_MODER_MODE12;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE13;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE14;
}

uint8_t ReadPinsB121314(void)
{
    uint8_t result = 0;
    
    if (GPIOB->IDR & GPIO_IDR_ID12)
        result |= 1 << 0;
    
    if (GPIOB->IDR & GPIO_IDR_ID13)
        result |= 1 << 1;
    
    if (GPIOB->IDR & GPIO_IDR_ID14)
        result |= 1 << 2;
    
    return result;
}
