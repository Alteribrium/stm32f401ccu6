#include "Engine2/Engine_B6B7.h" // ???????????, ??? ??? ????? ?? ??

void Engine_B6B7_init(void);
void Engine_B6B7_setPWM(float voltage);

void Engine_B6B7_init(void){
		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
			// PB6 -> TIM4_CH1
		GPIOB->MODER &= ~GPIO_MODER_MODER6;
		GPIOB->MODER |= GPIO_MODER_MODER6_1;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
		GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos);

		// PB7 -> TIM4_CH2
		GPIOB->MODER &= ~GPIO_MODER_MODER7;
		GPIOB->MODER |= GPIO_MODER_MODER7_1;
		GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7;
		GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL7_Pos);
		TIM4->PSC = 1600 - 1;
		TIM4->ARR = 100;
			// CH1
		TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;
		TIM4->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
		TIM4->CCMR1 |= TIM_CCMR1_OC1PE;

		// CH2
		TIM4->CCMR1 &= ~TIM_CCMR1_OC2M;
		TIM4->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos);
		TIM4->CCMR1 |= TIM_CCMR1_OC2PE;
		TIM4->CCER |= TIM_CCER_CC1E;
		TIM4->CCER |= TIM_CCER_CC2E;

		TIM4->CR1 |= TIM_CR1_ARPE;
		TIM4->CR1 &= ~TIM_CR1_DIR;
		TIM4->CR1 &= ~TIM_CR1_CMS;

		TIM4->EGR |= TIM_EGR_UG;
		TIM4->CR1 |= TIM_CR1_CEN;

		TIM4->CCR1 = 0;
		TIM4->CCR2 = 0;
}

void Engine_B6B7_setPWM(float voltage){
    int16_t PWM = (int16_t)(voltage * 100 / MAXVOLTAGE_Engige_B6B7);
    if(voltage >= 0){
        TIM4->CCR1 = (uint32_t)(PWM);
        TIM4->CCR2 = 0;
    }
    else{
        TIM4->CCR1 = 0;
        TIM4->CCR2 = (uint32_t)((-1) * PWM);
    }
}
