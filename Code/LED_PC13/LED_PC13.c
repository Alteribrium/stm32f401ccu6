#include "LED_PC13.h"

void LED_PC13_toggle(void);
void LED_PC13_on(void);
void LED_PC13_off(void);
void LED_PC13_init(void);
void TIM1_BRK_TIM9_IRQHandler(void);
void transfer_PA0_to_PC13(void);
void LED_PC13_brightness(uint32_t brightness);
uint32_t LED_PC13_getbrightness(void);


void LED_PC13_init(void){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= (1 << GPIO_MODER_MODER13_Pos);
	GPIOC->ODR |= GPIO_ODR_OD13;
	
	/*
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODER13;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
	*/
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	TIM9->PSC = 1600 - 1;
	TIM9->ARR = 100;
	GPIOA->MODER |= GPIO_MODER_MODE2_1;  //alter
	GPIOA->AFR[0] |= 3 << GPIO_AFRL_AFSEL2_Pos;
	TIM9->CCER |= TIM_CCER_CC1E;
	TIM9->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	TIM9->CCR1 = 60;
	TIM9->EGR |= TIM_EGR_UG;
	TIM9->SR = ~TIM_SR_UIF;
	TIM9->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE;
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	TIM9->CR1 |= TIM_CR1_CEN;
}

/*
void LED_PC13_toggle(void){
	GPIOC->ODR ^= GPIO_ODR_OD13;
}
*/

//if (TIM9->CR1 & TIM_CR1_CEN)

void LED_PC13_off(void){
	TIM9->CR1 &= ~TIM_CR1_CEN;
	GPIOC->ODR |= GPIO_ODR_OD13;
}

void LED_PC13_on(void){
	TIM9->CR1 |= TIM_CR1_CEN;
}

void LED_PC13_brightness(uint32_t brightness){
	TIM9->CCR1 = brightness;
}

uint32_t LED_PC13_getbrightness(void){
	return (TIM9->CCR1);
}

void TIM1_BRK_TIM9_IRQHandler(void) {
	uint32_t tim9Status = TIM9->SR;
	if(tim9Status & TIM_SR_UIF) {
		TIM9->SR = ~TIM_SR_UIF;
		GPIOC->ODR &= ~GPIO_ODR_OD13;
		//LED_PC13_on();
	}

	if(tim9Status & TIM_SR_CC1IF) {
		TIM9->SR = ~TIM_SR_CC1IF;
		GPIOC->ODR |= GPIO_ODR_OD13;
		//LED_PC13_off();
		}

}
