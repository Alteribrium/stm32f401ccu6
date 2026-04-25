#include "PA4.h"

static volatile uint8_t table_running = 0;
static uint16_t current_period = 100;

void Dispenser_A4_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    GPIOA->MODER &= ~GPIO_MODER_MODER4;
    GPIOA->MODER |= GPIO_MODER_MODER4_1; 
    
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL4;
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL4_1;
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    
    TIM3->PSC = 16 - 1;
    TIM3->ARR = current_period;
    TIM3->CCR3 = current_period / 2; 
    
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
    
    TIM3->CCER |= TIM_CCER_CC3E;
    
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Start_Dispenser_A4(void) {
    if (!table_running) {
        table_running = 1;
        TIM3->CNT = 0;
        TIM3->CR1 |= TIM_CR1_CEN;
    }
}

void Stop_Dispenser_A4(void) {
    if (table_running) {
        table_running = 0;
        TIM3->CR1 &= ~TIM_CR1_CEN;
        GPIOA->BSRR = GPIO_BSRR_BR_4;
    }
}
