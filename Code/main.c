#include "main.h"

uint8_t odrstatus;

void init(void);

int main(void)
{ 
	init();
	
	for(;;) {
		
		 if (USART1->SR & USART_SR_RXNE) {
        // ?????? DR ????????????? ?????????? RXNE
        uint8_t received = USART1->DR;
        
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = received;
    }
		
	}
}

		/*
		
		odrstatus = (GPIOA->ODR & (1 << 9)) != 0;
		if (Flag_ControlSysetmA2A3){
			Flag_ControlSysetmA2A3 = 0;
			ControlSystemA2A3();
		}
		targetSpeedA2A3 = OutputRegisters[0];
		Modbus_Work();*/

void init(void){
	SysClock_init();
	Engine_A2A3_init();
	USART1_init();
}
