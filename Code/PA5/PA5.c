#include "PA5.h"

static volatile uint8_t table_running = 0;
static uint16_t current_period = 100;

void Dispenser_A5_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_1; 
    
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5;
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_1;
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    
    TIM3->PSC = 16 - 1;
    TIM3->ARR = current_period;
    TIM3->CCR2 = current_period / 2;
    
    TIM3->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE; 
    
    TIM3->CCER |= TIM_CCER_CC2E;
    
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Start_Dispenser_A5(void) {
    if (!table_running) {
        table_running = 1;
        TIM3->CNT = 0;
        TIM3->CR1 |= TIM_CR1_CEN;
    }
}

void Stop_Dispenser_A5(void) {
    if (table_running) {
        table_running = 0;
        TIM3->CR1 &= ~TIM_CR1_CEN;
        GPIOA->BSRR = GPIO_BSRR_BR_5;
    }
}
