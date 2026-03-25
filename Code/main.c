#include "main.h"
void init(void);

int main(void)
{ 
	init();
	for(;;){
		ControlSystemA2A3();
		//targetSpeedA2A3 = OutputRegisters[0];
		Modbus_Work();
		WriteModbus();
	}
}

void init(void){
	SysClock_init();
	Engine_A2A3_init();
	USART1_init();
}
