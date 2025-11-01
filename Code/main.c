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
	SysClock_init();
	Engine_A2A3_init();
}
