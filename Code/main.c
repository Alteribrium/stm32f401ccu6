#include "main.h"
void init(void);
void Conveyor_Process(void);

typedef enum {
    CONVEYOR_STOP = 0,
    CONVEYOR_RUN = 1
} ConveyorState_t;

static ConveyorState_t conveyorState = CONVEYOR_STOP;
static uint8_t prevconveyorState = 255;



int main(void)
{ 
	init();
	for(;;){
			Conveyor_Process();
			Modbus_Work();
			WriteModbus();
	}
}



void Conveyor_Process(void){
	if(conveyorState != prevconveyorState){
			prevconveyorState = conveyorState;
					if(conveyorState == CONVEYOR_RUN){StartConveyor();}
					else{StopConveyor();}}
	switch(conveyorState)
			{case CONVEYOR_RUN:
            if (PB6_state) conveyorState = CONVEYOR_STOP; break;
			case CONVEYOR_STOP:
            if (!PB6_state) conveyorState = CONVEYOR_RUN; break;
			}
}


void init(void){
		BUTTON_PB6_Init();
		SysClock_init();
		Engine_A2A3_init();
		USART1_init();
}
