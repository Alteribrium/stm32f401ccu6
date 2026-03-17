#include "main.h"
void init(void);

int main(void)
{ 
	init();
	for(;;){
		/*
		if (Flag_ControlSysetmA2A3){
			Flag_ControlSysetmA2A3 = 0;
			ControlSystemA2A3();
		}
		*/
		//targetSpeedA2A3 = OutputRegisters[0];
		Modbus_Work();
	}
}

void init(void){
	SysClock_init();
	Engine_A2A3_init();
	USART1_init();
}
