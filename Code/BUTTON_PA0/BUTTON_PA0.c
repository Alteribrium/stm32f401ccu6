#include "BUTTON_PA0.h"

void BUTTON_PA0_Init(void);
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);

static uint8_t drebezg;

void BUTTON_PA0_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~GPIO_MODER_MODER0;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;
	
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
    EXTI->RTSR &= ~EXTI_RTSR_TR0; //up
	  EXTI->FTSR |= EXTI_FTSR_TR0;  //down
    EXTI->IMR |= EXTI_IMR_MR0;
		NVIC_SetPriority(EXTI0_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_IRQn);
	
	
	
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		TIM2->PSC = 16000;
		TIM2->ARR = 1000;
		TIM2->EGR |= TIM_EGR_UG;
		TIM2->SR &= ~TIM_SR_UIF;
		TIM2->DIER = TIM_DIER_UIE;
		NVIC_EnableIRQ(TIM2_IRQn);
	
}

void EXTI0_IRQHandler(void) { 
    if (EXTI->PR & EXTI_PR_PR0) { //BUTTON_PA0_Handler
				EXTI->PR = EXTI_PR_PR0;
				if(drebezg == 0){
					drebezg = 1;
					LED_PC13_brightness(0);					
					TIM2->ARR = 500;  //time
					TIM2->CNT = 0;
					TIM2->CR1 |= TIM_CR1_CEN;
				}
    }
}

void TIM2_IRQHandler(void) { //drebezg
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
				drebezg = 0;
				TIM2->CR1 &= ~TIM_CR1_CEN;	
				TIM2->CNT = 0;
    }
}
