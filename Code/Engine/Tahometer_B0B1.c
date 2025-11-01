#include "Engine/Tahometer_B0B1.h"

void Tahometer_B0B1_init(void);
void TIM3_IRQHandler(void);
void Tahometer_B0B1_riseDetected(uint32_t value,int8_t dir);
void Tahometer_B0B1_zeroriseDetected(void);
void TIM2_IRQHandler(void);

static uint32_t time[10] = {0};
static int8_t direction[10] = {0};
static uint8_t pointer = 0;
static uint32_t prevValue = 0;
static uint8_t pause = 0;
int32_t speed = 0;


void Tahometer_B0B1_init(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 500;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->EGR |= TIM_EGR_UG;
  NVIC_EnableIRQ(TIM2_IRQn);
	
	TIM2->CR1 |= TIM_CR1_CEN;



	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	GPIOB->MODER &= ~GPIO_MODER_MODER1;
	
	GPIOB->MODER &= ~GPIO_MODER_MODER0;
	GPIOB->MODER |= GPIO_MODER_MODER0_1;
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL0;
	GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL0_Pos);
	
	
	TIM3->PSC = 16-1;
	TIM3->ARR = 12000;
	
	TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM3->CCMR2 |= TIM_CCMR2_CC3S_0;


	TIM3->CCER &= ~TIM_CCER_CC3P;
	TIM3->CCER &= ~TIM_CCER_CC3NP;
	
	TIM3->CCER |= TIM_CCER_CC3E;
	
	TIM3->CCMR2 &= ~TIM_CCMR2_IC3F;
	
	TIM3->DIER |= TIM_DIER_CC3IE;
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 1);

	TIM3->CR1 |= TIM_CR1_CEN;
}


void TIM3_IRQHandler(void){
	if(TIM3->SR & TIM_SR_CC3IF){
		TIM3->SR &= ~TIM_SR_CC3IF;
			if (GPIOB->IDR & GPIO_IDR_ID1){
				Tahometer_B0B1_riseDetected(TIM3->CCR3,-1);
			}
			else{
				Tahometer_B0B1_riseDetected(TIM3->CCR3,1);
			}
	}
	if(TIM3->SR & TIM_SR_UIF){
		TIM3->SR &= ~TIM_SR_UIF;
		{
			Tahometer_B0B1_zeroriseDetected();
		}
	}
}


void Tahometer_B0B1_zeroriseDetected(void){
	if(pause == 0){
		pause++;
	}
	else{
		time[pointer] = 0;
		direction[pointer] = 0;
		pointer = ++pointer == 10 ? 0 : pointer;
	}
}


void Tahometer_B0B1_riseDetected(uint32_t value,int8_t dir){
	time[pointer] = value - prevValue;
	pause = 0;
	direction[pointer] = dir;
	prevValue = value;
	pointer = ++pointer == 10 ? 0 : pointer;
}

void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;
		if (direction[0] == 0 || direction[1] == 0 ||direction[2] == 0 ||direction[3] == 0 ||direction[4] == 0 ||direction[5] == 0 ||direction[6] == 0 ||direction[7] == 0 ||direction[8] == 0 ||direction[9] == 0){
			speed = 0;
		}
		else{
			speed = (int32_t)(SpeedScaler / (time[0] + time[1] + time[2] + time[3] + time[4] + time[5] + time[6] + time[7] + time[8] + time[9])) * (direction[0] + direction[1] + direction[2] + direction[3] + direction[4] + direction[5] + direction[6] + direction[7] + direction[8] + direction[9] > 0 ? 1 : -1);
		}
	}
}
