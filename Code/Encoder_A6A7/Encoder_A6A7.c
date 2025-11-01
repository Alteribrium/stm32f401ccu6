#include "Encoder_A6A7.h"

void Encoder_A6A7_init(void);
void EXTI9_5_IRQHandler(void);
void TIM5_IRQHandler(void);
void StartDrebezg_Encoder_B0(void);

//static uint32_t brightness = 0;
static uint8_t drebezg = 0;

void Encoder_A6A7_init(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		GPIOA->MODER &= ~GPIO_MODER_MODER7;
		GPIOA->MODER &= ~GPIO_MODER_MODER6;

		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
		SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;
	
		EXTI->RTSR &= ~EXTI_RTSR_TR6;
		EXTI->FTSR |= EXTI_FTSR_TR6;
		EXTI->IMR |= EXTI_IMR_MR6;
	
		EXTI->RTSR &= ~EXTI_RTSR_TR7;
		EXTI->FTSR |= EXTI_FTSR_TR7;
		EXTI->IMR |= EXTI_IMR_MR7;
	
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
		TIM5->PSC = 16000;
		TIM5->ARR = 1000;
		TIM5->EGR |= TIM_EGR_UG;
		TIM5->SR &= ~TIM_SR_UIF;
		TIM5->DIER = TIM_DIER_UIE;
		NVIC_EnableIRQ(TIM5_IRQn);
	
		NVIC_SetPriority(EXTI9_5_IRQn, 0);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(void){
		
	if(EXTI->PR & EXTI_PR_PR7){ // clockwise
			EXTI->PR = EXTI_PR_PR7;
		if(drebezg == 0){
			StartDrebezg_Encoder_B0();
			//brightness = LED_PC13_getbrightness();
			if ((GPIOA->IDR & GPIO_IDR_ID6) ? 1 : 0 == 1)
			{
				if (TIM9->CCR1 + 10 <= 100){
					LED_PC13_brightness(TIM9->CCR1 + 10); //brightness??
				}
				else if(TIM9->CCR1 < 100){
					LED_PC13_brightness(100);
				}
			}
		}
	}
	if(EXTI->PR & EXTI_PR_PR6){ // !clockwise
			EXTI->PR = EXTI_PR_PR6;
			//brightness = LED_PC13_getbrightness();
			if(drebezg == 0){
				StartDrebezg_Encoder_B0();
				if ((GPIOA->IDR & GPIO_IDR_ID7) ? 1 : 0 == 1)
				{
					if (TIM9->CCR1 >= 15){
						LED_PC13_brightness(TIM9->CCR1 - 15); //brightness??
					}
					else if(TIM9->CCR1 < 15){
						LED_PC13_brightness(0);
					}
				}
			}
	}
}

void TIM5_IRQHandler(void) {
    if (TIM5->SR & TIM_SR_UIF) {
        TIM5->SR &= ~TIM_SR_UIF;
				drebezg = 0;
				TIM5->CR1 &= ~TIM_CR1_CEN;	
				TIM5->CNT = 0;
    }
}

void StartDrebezg_Encoder_B0(void){
	drebezg = 1;
	TIM5->ARR = 50;  //time (50)
	TIM5->CNT = 0;
	TIM5->CR1 |= TIM_CR1_CEN;
}
