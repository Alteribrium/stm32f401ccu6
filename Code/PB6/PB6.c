#include "PB6.h"

void BUTTON_PB6_Init(void);
void EXTI9_5_IRQHandler(void);
void TIM2_IRQHandler(void);
uint8_t PB6_state;
static uint8_t drebezg;

void BUTTON_PB6_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    
    GPIOB->MODER &= ~GPIO_MODER_MODER6;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PB;

    EXTI->RTSR |= EXTI_RTSR_TR6;
    EXTI->FTSR |= EXTI_FTSR_TR6;
    EXTI->IMR |= EXTI_IMR_MR6;

    NVIC_SetPriority(EXTI9_5_IRQn, 0);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
	
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 16000;
    TIM2->ARR = 1000;
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
		
		PB6_state = 0;
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR6) {
        EXTI->PR = EXTI_PR_PR6;

        if (drebezg == 0) {
            drebezg = 1;

            if(GPIOB->IDR & GPIO_IDR_IDR_6){
							PB6_state = 1;
						}
						else{
							PB6_state = 0;
						}
            TIM2->ARR = 500;
            TIM2->CNT = 0;
            TIM2->CR1 |= TIM_CR1_CEN;
        }
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        drebezg = 0;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        TIM2->CNT = 0;
    }
}
