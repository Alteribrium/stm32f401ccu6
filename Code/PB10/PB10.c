#include "PB10.h"


void BUTTON_PB10_Init(void);
void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);

uint8_t PB10_state;
static uint8_t drebezg;

void BUTTON_PB10_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    
    // PB10 input
    GPIOB->MODER &= ~GPIO_MODER_MODER10;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR10;
		GPIOB->PUPDR |= GPIO_PUPDR_PUPDR10_0;

    // EXTI ?????????
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI10;   // ???????
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PB; // PB10

    EXTI->RTSR |= EXTI_RTSR_TR10;
    EXTI->FTSR |= EXTI_FTSR_TR10;
    EXTI->IMR  |= EXTI_IMR_MR10;

    // ? ?????? IRQ
    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
	
    // ?????? ??? ????????????
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 16000;
    TIM2->ARR = 1000;
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->DIER = TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM2_IRQn);
		
    PB10_state = 0;
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR10) {
        EXTI->PR = EXTI_PR_PR10;

        if (drebezg == 0) {
            drebezg = 1;

            if(GPIOB->IDR & GPIO_IDR_IDR_10){
                PB10_state = 1;
            } else {
                PB10_state = 0;
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
