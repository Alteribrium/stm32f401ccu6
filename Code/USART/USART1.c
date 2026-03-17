#include "USART.h"
#include <stdio.h>
#include <string.h>

/*
* USART1 configure for modbus
* PA9 - TX
* PA10 - RX
* PA8 - toggle driver to reveive/transmite mode
*/

void Modbus_Send_Callback(int8_t length);
void USART1_DMA_init(void);

void USART1_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);
void USART1_DMA_SendString(char *str, uint16_t l);

//DMA configure
static uint8_t dma_rx_buffer[256];
static uint8_t dma_tx_buffer[256];
static  uint8_t dma_tx_busy = 0;
extern uint8_t buff_rx_len;
uint8_t buff_rx_len = 0;
extern uint8_t receive_complete;
uint8_t receive_complete = 0;

//MODBUS configure
static uint8_t modbus_rx_buffer[256];
static uint8_t modbus_tx_buffer[256];
static uint8_t rx_index = 0;


void USART1_init(void){
	// PA8 - TOGGLER
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER8_1;
	GPIOA->ODR &= ~(1 << 8); //usart receive	
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1; // PA9 - TX
	GPIOA->MODER |= GPIO_MODER_MODER10_1; // PA10 - RX
	GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos) | (7 << GPIO_AFRH_AFSEL10_Pos);

	USART1->BRR = 1667; //9600 boudrate
	//USART1->BRR = 139; //115200 boudrate
	
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
	USART1_DMA_init();
	USART1->CR1 |= USART_CR1_IDLEIE | USART_CR1_TCIE;
	NVIC_EnableIRQ(USART1_IRQn);
}


void USART1_IRQHandler(void) {
	if (USART1->SR & USART_SR_IDLE){
		USART1->SR &= ~USART_SR_IDLE;
    volatile uint32_t temp = USART1->DR;
    temp = USART1->SR;
    (void)temp;
    buff_rx_len = 256 - (uint16_t)DMA2_Stream2->NDTR;
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    DMA2->LIFCR = DMA_LIFCR_CTCIF2;
    DMA2_Stream2->NDTR = 256;
    DMA2_Stream2->CR |= DMA_SxCR_EN;
    receive_complete = 1;
	}
	if (USART1->SR & USART_SR_TC) {
			USART1->SR &= ~USART_SR_TC; 
			GPIOA->ODR &= ~(1 << 8); //usart receive
	}
}


void USART1_DMA_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; 
	DMA2_Stream2->CR &= ~DMA_SxCR_EN;  // RX Stream2
	DMA2_Stream7->CR &= ~DMA_SxCR_EN;
	
	DMA2_Stream2->PAR = (uint32_t)&USART1->DR;
	DMA2_Stream2->M0AR = (uint32_t)dma_rx_buffer;
	DMA2_Stream2->NDTR = sizeof(dma_rx_buffer);
	DMA2_Stream2->CR = (4 << DMA_SxCR_CHSEL_Pos) |   // Channel 4 (USART1_RX)
													DMA_SxCR_MINC ;   // Memory increment
												//DMA_SxCR_TCIE;     // Transfer complete interrupt
	
	DMA2_Stream7->PAR = (uint32_t)&USART1->DR;
	DMA2_Stream7->M0AR = (uint32_t)dma_tx_buffer;
	DMA2_Stream7->NDTR = 0;
	DMA2_Stream7->CR = (4 << DMA_SxCR_CHSEL_Pos) |   // Channel 4 (USART2_TX) //4 << DMA_SxCR_CHSEL_Pos
										DMA_SxCR_MINC |
										DMA_SxCR_DIR_0 |      // Memory to peripheral
										DMA_SxCR_TCIE;        // Transfer complete interrupt
	
	USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);  // TX
 // NVIC_EnableIRQ(DMA2_Stream2_IRQn); // RX
	DMA2_Stream2->CR |= DMA_SxCR_EN; 
}


void DMA2_Stream7_IRQHandler(void) {
    if (DMA2->HISR & DMA_HISR_TCIF7) {
        DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
        dma_tx_busy = 0;
    }
}


void USART1_DMA_SendString(char *str, uint16_t l) {
    uint64_t startTime = msCounter;
    while (dma_tx_busy) {
        if (msCounter - startTime > 1000)
            break;
    }
		GPIOA->ODR |= (1 << 8); //usart transmite		
		
    dma_tx_busy = 1;
    memcpy(dma_tx_buffer, str, l);
    DMA2_Stream7->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream7->CR & DMA_SxCR_EN);
    DMA2_Stream7->M0AR = (uint32_t)dma_tx_buffer;
    DMA2_Stream7->NDTR = l;
    DMA2->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CTEIF7 | 
                  DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;
    DMA2_Stream7->CR |= DMA_SxCR_EN;
}


void Modbus_Work(void){
 if(receive_complete) {
		ModBusRTU_PR(dma_rx_buffer,	buff_rx_len,	modbus_tx_buffer,	Modbus_Send_Callback);
		receive_complete = 0;
	}
}


void Modbus_Send_Callback(int8_t length) {
	USART1_DMA_SendString(modbus_tx_buffer,length);
}


