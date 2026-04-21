#include "main.h"
void init(void);
void Conveyor_Process(void);
void Table_Process(void);
void Manipualtor_Process(void);
void HMI_Process(void);
static uint8_t pb10 = 0;
///
void StopTable(void);
void StartTable(void);

void Dispenser1start(void);
void Dispenser2start(void);
void Dispenser3start(void);

void ManipulatorGetFromTable(void);
void ManipulatorGetFromConveyor(void);
///
///
static uint8_t TableSensor = 0; //1 if rised, else 0
static uint8_t cup[5] = {0, 0, 0, 0, 0}; // 0 - no cup, 1 - cup; 0 - starting position , 5 - ending position
static uint8_t ConveyorSensor = 0; //1 if rised, else 0
static uint8_t targetCounter = 50;

static uint8_t Dispenser1state = 0;
static uint8_t Dispenser2state = 0;
static uint8_t Dispenser3state = 0;
///

typedef enum {
    MANIPULATOPSTOP = 0,
    MANIPULATORPICKCUPFROMTABLE = 1,
		MANIPULATORGETCUPFROMTABLE = 2,
		MANIPULATORGETCUPFROMTABLECOMPLETE = 3,
		MANIPULATORPICKCUPFROMCONVEYOR = 4,
		MANIPULATORGETCUPFROMCONVEYOR = 5,
		MANIPULATORGETCUPFROMCONVEYORCOMPLETE = 6
} ManipulatorState_t;

typedef enum {
    CONVEYOR_STOP = 0,
    CONVEYOR_RUN = 1
} ConveyorState_t;

typedef enum {
    TABLE_STOP = 0,
    TABLE_RUN = 1
} TableState_t;

static TableState_t tableState = TABLE_STOP;
static ConveyorState_t conveyorState = CONVEYOR_STOP;
static ManipulatorState_t manipulatorState = MANIPULATOPSTOP;
static uint8_t prevconveyorState = 255;
static uint8_t prevtableState = 255;



int main(void)
{ 
	init();
	for(;;){
			pb10 = (GPIOB->IDR & GPIO_IDR_IDR_10) ? 1 : 0;
		  HMI_Process();
			Manipualtor_Process();
			Table_Process();
			Conveyor_Process();
			Modbus_Work();
			WriteModbus();
	}
}

void HMI_Process(void){
	__NOP();
}


void Table_Process(void){
	if(tableState != prevtableState){
			prevtableState = tableState;
					if(tableState == TABLE_RUN){StartTable();}
					else{StopTable();}}
	switch(tableState){
		case TABLE_RUN:
			if(TableSensor){
				TableSensor = 0;
				cup[4] = cup[3]; cup[3] = cup[2]; cup[2] = cup[1]; cup[1] = cup[0]; cup[0] = 0;
				tableState = TABLE_STOP;
				if(cup[1]){Dispenser1start();}
				if(cup[2]){Dispenser2start();}
				if(cup[3]){Dispenser3start();}}break;
		case TABLE_STOP:
			if(cup[4] == 0 && Dispenser1state == 0 && Dispenser2state == 0 && Dispenser3state == 0 && (cup[0] || cup[1] || cup[2] || cup[3])){
				tableState = TABLE_RUN;}break;
	
	}
}


void Manipualtor_Process(void){
	if(manipulatorState == MANIPULATOPSTOP){
		if((targetCounter - cup[0] - cup[1] - cup[2] - cup[3] > 0) && tableState == TABLE_STOP && cup[0] == 0){
				manipulatorState = MANIPULATORGETCUPFROMCONVEYOR;
				ManipulatorGetFromConveyor();
		}
		else if (tableState == TABLE_STOP && cup[4] == 1){
				manipulatorState = MANIPULATORGETCUPFROMTABLE;
				ManipulatorGetFromTable();
		}
	else if (manipulatorState == MANIPULATORGETCUPFROMTABLECOMPLETE){
		manipulatorState = MANIPULATOPSTOP;
		cup[4] = 0;
	}
	else if (manipulatorState == MANIPULATORGETCUPFROMCONVEYORCOMPLETE){
		manipulatorState = MANIPULATOPSTOP;
		cup[0] = 1;
	}
	}
}

void Conveyor_Process(void){
	if(conveyorState != prevconveyorState){
			prevconveyorState = conveyorState;
					if(conveyorState == CONVEYOR_RUN){StartConveyor();}
					else{StopConveyor();}}
	switch(conveyorState)
			{case CONVEYOR_RUN:
            if (ConveyorSensor) conveyorState = CONVEYOR_STOP; break;
			case CONVEYOR_STOP:
            if (!(manipulatorState == MANIPULATORPICKCUPFROMCONVEYOR)) conveyorState = CONVEYOR_RUN; break;
			}
}


void init(void){
		//BUTTON_PB6_Init();
		BUTTON_PB10_Init();
		SysClock_init();
		Engine_A2A3_init();
		USART1_init();
		Engine_B6B7_init();
		Engine_B6B7_setPWM(0);
}
