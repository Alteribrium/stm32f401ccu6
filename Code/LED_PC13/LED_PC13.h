#ifndef LED_PC13_H_
#define LED_PC13_H_

#include "main.h"

void LED_PC13_toggle(void);
void LED_PC13_on(void);
void LED_PC13_off(void);
void LED_PC13_init(void);
void TIM1_BRK_TIM9_IRQHandler(void);
void transfer_PA0_to_PC13(void);
void LED_PC13_brightness(uint32_t brightness);
uint32_t LED_PC13_getbrightness(void);

#endif
