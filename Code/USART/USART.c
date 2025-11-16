#include "USART.h"

char temp = 0;

void USART2_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	

	
	GPIOA->MODER |= GPIO_MODER_MODER2_1; //PA2 TX
	GPIOA->MODER |= GPIO_MODER_MODER3_1; //PA3 RX
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
	
	USART2->BRR = 139;
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
}


void USART2_SendChar(uint8_t ch){
	while(!(USART2->SR & USART_SR_TXE));
	USART2->DR = ch;
}

void USART2_SendString(char *str){
	while(*str){
		USART2_SendChar(*str++);
	}
}


void USART2_IRQHandler(void){
	if(USART2->SR & USART_SR_RXNE){
		temp = USART2->DR;
		USART2_SendChar(temp);
	}
}
