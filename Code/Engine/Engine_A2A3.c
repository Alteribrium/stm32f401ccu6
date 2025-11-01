#include "Engine/Engine_A2A3.h"

void Engine_A2A3_init(void);
void Engine_A2A3_setPWM(float voltage);

void Engine_A2A3_init(void){
	
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    
    GPIOA->MODER &= ~GPIO_MODER_MODER2;
    GPIOA->MODER |= GPIO_MODER_MODER2_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
    GPIOA->AFR[0] |= (3 << GPIO_AFRL_AFSEL2_Pos);
    
    GPIOA->MODER &= ~GPIO_MODER_MODER3;
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
    GPIOA->AFR[0] |= (3 << GPIO_AFRL_AFSEL3_Pos);
    
    TIM9->PSC = 1600 - 1;
    TIM9->ARR = 100;
    
    TIM9->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM9->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM9->CCMR1 |= TIM_CCMR1_OC1PE;
    
    TIM9->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM9->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos);
    TIM9->CCMR1 |= TIM_CCMR1_OC2PE;
    
    TIM9->CCER |= TIM_CCER_CC1E;
    TIM9->CCER |= TIM_CCER_CC2E;
    
    TIM9->EGR |= TIM_EGR_UG;
    TIM9->CR1 |= TIM_CR1_CEN;
    
    TIM9->CCR1 = 0;
    TIM9->CCR2 = 50;
}


void Engine_A2A3_setPWM(float voltage){
	uint8_t PWM = (uint8_t)(voltage * 100 / MAXVOLTAGE_Engige_A2A3);
	if(voltage > 0){
		TIM9->CCR1 = PWM;
    TIM9->CCR2 = 0;
	}
	else{
		TIM9->CCR1 = 0;
    TIM9->CCR2 = PWM;
	}
}

