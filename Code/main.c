#include "main.h"

void init(void);

int main(void)
{ 
	init();
	
	for(;;) {
		__NOP();
	}
}

void init(void){
	//SysClock_init();
	//BUTTON_PA0_Init();
	LED_PC13_init();
	Encoder_A6A7_init();
}
