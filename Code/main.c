#include "main.h"

void init(void);

int main(void)
{ 
	init();
	
	for(;;) {
		Modbus_Work();
	}
}

void init(void){
	SysClock_init();
	USART2_init();
}
