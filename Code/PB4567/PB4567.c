#include "PB4567.h"

static volatile uint8_t table_running = 0;
extern uint8_t counter = 0;
void TIM4_IRQHandler(void);

static uint8_t states[4][8] = {
    {1, 1, 0, 0, 0, 0, 0, 1},
		{0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1}
};

void Table_B4567_Init(void) {
///
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

		TIM4->PSC = 800 - 1;     
		TIM4->ARR = 50;           
		TIM4->DIER |= TIM_DIER_UIE; 
		TIM4->EGR |= TIM_EGR_UG;   
		NVIC_EnableIRQ(TIM4_IRQn);
		TIM4->CR1 |= TIM_CR1_CEN;
///	
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		GPIOB->MODER &= ~GPIO_MODER_MODE4;
		GPIOB->MODER |= GPIO_MODER_MODE4_0; 
	
		//GPIOB->BSRR = GPIO_BSRR_BS4;
		//GPIOB->BSRR = GPIO_BSRR_BR4;
		GPIOB->MODER &= ~GPIO_MODER_MODE5;
		GPIOB->MODER |= GPIO_MODER_MODE5_0; 
	
		GPIOB->MODER &= ~GPIO_MODER_MODE6;
		GPIOB->MODER |= GPIO_MODER_MODE6_0;
	
		GPIOB->MODER &= ~GPIO_MODER_MODE7;
		GPIOB->MODER |= GPIO_MODER_MODE7_0; 
	
		GPIOB->BSRR = GPIO_BSRR_BR4;
		GPIOB->BSRR = GPIO_BSRR_BR5;
		GPIOB->BSRR = GPIO_BSRR_BR6;
		GPIOB->BSRR = GPIO_BSRR_BR7;
	
    Stop_table_B4567();
}

void Start_table_B4567(void) {
    if (!table_running) {
        table_running = 1;
    }
}

void Stop_table_B4567(void) {
    if (table_running) {
        table_running = 0;
			GPIOB->BSRR = GPIO_BSRR_BR4;
			GPIOB->BSRR = GPIO_BSRR_BR5;
			GPIOB->BSRR = GPIO_BSRR_BR6;
			GPIOB->BSRR = GPIO_BSRR_BR7;
    }
}

void table_B4567_Process(void){
		if(table_running){
			if(states[0][counter%8]){
				GPIOB->BSRR = GPIO_BSRR_BS4;
			}
			else{
				GPIOB->BSRR = GPIO_BSRR_BR4;
			}
			if(states[1][counter%8]){
				GPIOB->BSRR = GPIO_BSRR_BS5;
			}
			else{
				GPIOB->BSRR = GPIO_BSRR_BR5;
			}
			if(states[2][counter%8]){
				GPIOB->BSRR = GPIO_BSRR_BS6;
			}
			else{
				GPIOB->BSRR = GPIO_BSRR_BR6;
			}
			if(states[3][counter%8]){
				GPIOB->BSRR = GPIO_BSRR_BS7;
			}
			else{
				GPIOB->BSRR = GPIO_BSRR_BR7;
			}
			counter++;
			if (counter == 8){counter = 0;}
		}
		else{
			__NOP();
		}
}


void TIM4_IRQHandler(void) {
    if(TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;
        if(table_running) {
            table_B4567_Process();
        }
    }
}
