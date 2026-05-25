#include "PB8.h"


void BUTTON_PB10_Init(void);
void TIM1_UP_TIM10_IRQHandler(void);


uint8_t TableSensor = 0;
static uint8_t drebezg;

void BUTTON_PB8_Init(void) {

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    

    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->PUPDR &= ~(3 << (8 * 2)); 
    
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB; 
    

    EXTI->RTSR |= EXTI_RTSR_TR8;   
    EXTI->FTSR &= ~EXTI_FTSR_TR8;   
    

    EXTI->IMR |= EXTI_IMR_MR8;
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; 
		TIM1->PSC = 16000;                   
		TIM1->ARR = 1000;                   
		TIM1->EGR |= TIM_EGR_UG;             
		TIM1->SR &= ~TIM_SR_UIF;            
		TIM1->DIER = TIM_DIER_UIE;   
    
    NVIC_EnableIRQ(EXTI9_5_IRQn); 
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}


void EXTI9_5_IRQHandler(void) {

    if (EXTI->PR & EXTI_PR_PR8) {
        EXTI->PR |= EXTI_PR_PR8;
			if (drebezg == 0) {
            drebezg = 1;
            //TableSensor = 1;

            TIM1->ARR = 500;
            TIM1->CNT = 0;
            TIM1->CR1 |= TIM_CR1_CEN;
        }
			
    }
}

void TIM1_UP_TIM10_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF;
        drebezg = 0;
        TIM1->CR1 &= ~TIM_CR1_CEN;
        TIM1->CNT = 0;
				if(((GPIOB->IDR & GPIO_IDR_IDR_8) ? 1 : 0) == 1){
				TableSensor = 1;}
    }
}
