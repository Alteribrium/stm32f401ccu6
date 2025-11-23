#include "USART.h"
#include <stdio.h>
char temp = 0;

static uint8_t dma_tx_buffer[256];
static uint8_t dma_rx_buffer[256];
static uint8_t dma_tx_busy = 0;
static uint8_t dma_rx_ready = 0;
static uint32_t dma_rx_length = 0; 
static uint16_t len;

void USART2_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	

	
	GPIOA->MODER |= GPIO_MODER_MODER2_1; //PA2 TX
	GPIOA->MODER |= GPIO_MODER_MODER3_1; //PA3 RX
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
	
	USART2->BRR = 139;
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	
	USART2_DMA_init();
	
	//USART2->CR1 |= USART_CR1_RXNEIE;
	//NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_DMA_init(void){
	 RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)dma_rx_buffer;
    DMA1_Stream5->NDTR = sizeof(dma_rx_buffer);
    
    DMA1_Stream5->CR = (1 << 27) |        // Channel 4
                      DMA_SxCR_MINC |    // Memory increment
                      DMA_SxCR_TCIE;     // Transfer complete interrupt
    
    USART2->CR3 |= USART_CR3_DMAR;
    
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR = (uint32_t)dma_tx_buffer;
    DMA1_Stream6->NDTR = 0;
    
    DMA1_Stream6->CR = (1 << 27) |   // Channel 4 (USART2_TX)
                      DMA_SxCR_PL_0 |
                      DMA_SxCR_MSIZE_0 |
                      DMA_SxCR_PSIZE_0 |
                      DMA_SxCR_MINC |
                      DMA_SxCR_DIR_0 |      // Memory to peripheral
                      DMA_SxCR_TCIE;        // Transfer complete interrupt
    
    USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    
    NVIC_EnableIRQ(DMA1_Stream6_IRQn); // TX
    NVIC_EnableIRQ(DMA1_Stream5_IRQn); // RX
    
    DMA1_Stream5->CR |= DMA_SxCR_EN;
}

void USART2_DMA_SendChar(char str){
    while(dma_tx_busy);
    dma_tx_busy = 1;
		dma_tx_buffer[0] = str;
    
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while(DMA1_Stream6->CR & DMA_SxCR_EN);
    
    DMA1_Stream6->M0AR = (uint32_t)dma_tx_buffer;
    DMA1_Stream6->NDTR = 1;
    
    
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void USART2_DMA_SendString(char *str){
    while(dma_tx_busy);
		dma_tx_busy = 1;
    
    len = 0;
    while(str[len] != '\0'){
        dma_tx_buffer[len] = str[len];
        len++;
    }
    
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while(DMA1_Stream6->CR & DMA_SxCR_EN);
    
    DMA1_Stream6->M0AR = (uint32_t)dma_tx_buffer;
    DMA1_Stream6->NDTR = len;
    
    
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void DMA1_Stream6_IRQHandler(void){
    if(DMA1->HISR & DMA_HISR_TCIF6){
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
        dma_tx_busy = 0;
    }
}
/*
void DMA1_Stream5_IRQHandler(void){
    if(DMA1->LISR & (1 << 11)){  // TCIF5 - Transfer Complete
        DMA1->LIFCR |= (1 << 11); // Clear TCIF5
        
        dma_rx_length = sizeof(dma_rx_buffer);
        dma_rx_ready = 1;
        
        DMA1_Stream5->CR &= ~DMA_SxCR_EN;
        while(DMA1_Stream5->CR & DMA_SxCR_EN);
        
        DMA1_Stream5->NDTR = sizeof(dma_rx_buffer);
        DMA1_Stream5->CR |= DMA_SxCR_EN;
			
				USART2_Process_Received_Data();
    }
}

void USART2_Process_Received_Data(void){
	 
	//uint32_t data_length = dma_rx_length;
	USART2_DMA_SendString("12345");
	
}
*/