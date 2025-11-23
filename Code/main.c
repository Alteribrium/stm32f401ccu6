#include "main.h"

void init(void);

int main(void)
{ 
	init();
	
	for(;;) {
		//uint8_t buff[] = {1,2,3,4,5,6,7,8};
		USART2_DMA_SendString("asafas");
		SysClock_delay(1000);
	}
}

void init(void){
	SysClock_init();
	USART2_init();
}
