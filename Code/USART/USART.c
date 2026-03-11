#include "USART.h"
#include <stdio.h>
#include <string.h>
char temp = 0;

void USART2_IRQHandler(void);

void DMA1_Stream6_IRQHandler(void);

extern uint8_t receve_complete;
uint8_t receve_complete = 0;

extern uint16_t buff_rx_len;
uint16_t buff_rx_len = 0;
static uint8_t dma_tx_buffer[256];
static uint8_t dma_rx_buffer[256];
static uint8_t dma_tx_busy = 0;
extern uint8_t dma_rx_ready;
uint8_t dma_rx_ready = 0;
//static uint32_t dma_rx_length = 0; 
//static uint16_t len;



static uint8_t modbus_rx_buffer[256];
static uint8_t modbus_tx_buffer[256];
static uint8_t rx_index = 0;
void Modbus_Send_Callback(int8_t length);
void Modbus_Work(void);




// ???????? ???????? ??????
uint8_t RS485_IsTransmitMode(void) {
    return (GPIOA->ODR & (1 << 9)) != 0;
}


void USART2_init(void){
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA9 ?????? ?? ?????? ?????????, ? ?????????? RS485!
    GPIOA->MODER |= (1 << 18);     // PA9 ??? ????? (MODER9_0 = 1)
    GPIOA->MODER &= ~(1 << 19);    // PA9 ??? ????? (MODER9_1 = 0)
    GPIOA->OTYPER &= ~(1 << 9);    // Push-pull (?? ???????? ????)
    GPIOA->OSPEEDR |= (3 << 18);   // ??????? ???????? (???? ?????)
    
    GPIOA->ODR &= ~(1 << 9);
	
	//////
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	

	
	GPIOA->MODER |= GPIO_MODER_MODER2_1; //PA2 TX
	GPIOA->MODER |= GPIO_MODER_MODER3_1; //PA3 RX
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
	
	USART2->BRR = 139;
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	
	USART2_DMA_init();
	
	USART2->CR1 |= USART_CR1_IDLEIE;
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_DMA_init(void){
	 RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)dma_rx_buffer;
    DMA1_Stream5->NDTR = sizeof(dma_rx_buffer);
    
    DMA1_Stream5->CR = (1 << 27) |        // Channel 4
                      DMA_SxCR_MINC ;   // Memory increment
                      //DMA_SxCR_TCIE;     // Transfer complete interrupt
    
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR = (uint32_t)dma_tx_buffer;
    DMA1_Stream6->NDTR = 0;
    
    DMA1_Stream6->CR = (1 << 27) |   // Channel 4 (USART2_TX) //4 << DMA_SxCR_CHSEL_Pos
                      DMA_SxCR_MINC |
                      DMA_SxCR_DIR_0 |      // Memory to peripheral
                      DMA_SxCR_TCIE;        // Transfer complete interrupt
    
    USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    
    NVIC_EnableIRQ(DMA1_Stream6_IRQn); // TX
   // NVIC_EnableIRQ(DMA1_Stream5_IRQn); // RX
    
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

void USART2_DMA_SendString(char *str,uint16_t l){
		uint64_t startTime = msCounter;
		while(dma_tx_busy) {
			if(msCounter - startTime > 1000)
				break;
		}
		
		////
			//GPIOA->ODR |= (1 << 9);
    

    //SysClock_delay(1);
		
		/////
		
		dma_tx_busy = 1;
		memcpy(dma_tx_buffer , str, l); 
    /*while(str[len] != '\0'){
        dma_tx_buffer[len] = str[len];
        len++;
    }
    */
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while(DMA1_Stream6->CR & DMA_SxCR_EN);
    
    DMA1_Stream6->M0AR = (uint32_t)dma_tx_buffer;
    DMA1_Stream6->NDTR = l;
    
    
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void DMA1_Stream6_IRQHandler(void){
    if(DMA1->HISR & DMA_HISR_TCIF6){
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
        dma_tx_busy = 0;
			/////
			GPIOA->ODR &= ~(1 << 9);
			/////
				
    }
}
void USART2_IRQHandler(void){
	USART2->SR &= ~USART_SR_IDLE;
	volatile uint32_t temp = USART2->DR;
	temp = USART2->SR;
	(void)temp;
	buff_rx_len = 256 - (uint16_t)DMA1_Stream5->NDTR;
	DMA1_Stream5->CR &= ~DMA_SxCR_EN;
	DMA1->HIFCR = DMA_HIFCR_CTCIF5;
	DMA1_Stream5->NDTR = 256;
	DMA1_Stream5->CR |= DMA_SxCR_EN;
	receve_complete = 1;
}

void Modbus_Work(void){
 if(receve_complete) {
		ModBusRTU_PR(dma_rx_buffer,	buff_rx_len,	modbus_tx_buffer,	Modbus_Send_Callback);
		receve_complete = 0;
	}
}

void Modbus_Send_Callback(int8_t length) {
	USART2_DMA_SendString(modbus_tx_buffer,length);
}


