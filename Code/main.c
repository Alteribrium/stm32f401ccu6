#include "main.h"

void init(void);

int main(void)
{ 
	init();
	
	for(;;) {
		if (Flag_ControlSysetmA2A3){
			Flag_ControlSysetmA2A3 = 0;
			ControlSystemA2A3();
		}
	}
}

void init(void){
	SysClock_init();
	Engine_A2A3_init();
}
