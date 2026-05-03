#include "main.h"
void init(void);
void Conveyor_Process(void);
void Table_Process(void);
void Manipualtor_Process(void);
void HMI_Process(void);
void Dispenser1_Process(void);
void Dispenser2_Process(void);
void Dispenser3_Process(void);
///


void ManipulatorGetFromTable(void);
void ManipulatorGetFromConveyor(void);
///
///
static uint8_t cup[5] = {0, 0, 0, 0, 0}; // 0 - no cup, 1 - cup; 0 - starting position , 5 - ending position
static uint8_t ConveyorSensor = 0; //1 if rised, else 0
static uint8_t targetCounter = 50;

///




//
static uint64_t prevtime;

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
    DISPENSER_STOP = 0,
    DISPENSER_RUN = 1
} DispenserState_t;

typedef enum {
    TABLE_STOP = 0,
    TABLE_RUN = 1
} TableState_t;


static DispenserState_t Dispenser1state = DISPENSER_STOP;
static DispenserState_t Dispenser2state = DISPENSER_STOP;
static DispenserState_t Dispenser3state = DISPENSER_STOP;

static uint64_t Dispenser1startTime = 0;
static uint64_t Dispenser2startTime = 0;
static uint64_t Dispenser3startTime = 0;

static uint64_t Dispenser1targetTime = 60000;
static uint64_t Dispenser2targetTime = 60000;
static uint64_t Dispenser3targetTime = 60000;


static uint8_t prevDispenser1state = 255;
static uint8_t prevDispenser2state = 255;
static uint8_t prevDispenser3state = 255;

static TableState_t tableState = TABLE_STOP;
static ConveyorState_t conveyorState = CONVEYOR_STOP;
static ManipulatorState_t manipulatorState = MANIPULATOPSTOP;
static uint8_t prevconveyorState = 255;
static uint8_t prevtableState = 255;



int main(void)
{ 
	init();
	
	for(;;){
		  HMI_Process();
			Manipualtor_Process();
			Table_Process();
			Conveyor_Process();
			Dispenser1_Process();
			Dispenser2_Process();
			Dispenser3_Process();
			Modbus_Work();
			WriteModbus();
			if( msCounter - prevtime > 100){
				prevtime = msCounter;
				table_B4567_Process();
			}
	}
	/*
	for(;;){
		__NOP();
	}*/
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
				if(cup[1]){Dispenser1state = DISPENSER_RUN;}
				if(cup[2]){Dispenser2state = DISPENSER_RUN;}
				if(cup[3]){Dispenser3state = DISPENSER_RUN;}}break;
		case TABLE_STOP:
			if(cup[4] == 0 && Dispenser1state == 0 && Dispenser2state == 0 && Dispenser3state == 0 && 
				(cup[0] || cup[1] || cup[2] || cup[3]) && 
				( (cup[0] + cup[1] + cup[2] + cup[3] + cup[4] >= targetCounter) || (cup[0])))
			{tableState = TABLE_RUN;}break;
	
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




void Dispenser1_Process(void){
	if(Dispenser1state != prevDispenser1state){
			prevDispenser1state = Dispenser1state;
					if(Dispenser1state == DISPENSER_RUN){Dispenser1start(); Dispenser1startTime = msCounter;}
					else{Dispenser1stop();}}
	switch(Dispenser1state)
			{case DISPENSER_RUN:
            if (msCounter >= Dispenser1startTime + Dispenser1targetTime) Dispenser1state = DISPENSER_STOP; break;
			case DISPENSER_STOP:
            __NOP(); break;
			}
}

void Dispenser2_Process(void){
	if(Dispenser2state != prevDispenser2state){
			prevDispenser2state = Dispenser2state;
					if(Dispenser2state == DISPENSER_RUN){Dispenser2start(); Dispenser2startTime = msCounter;}
					else{Dispenser2stop();}}
	switch(Dispenser2state)
			{case DISPENSER_RUN:
            if (msCounter >= Dispenser2startTime + Dispenser2targetTime) Dispenser2state = DISPENSER_STOP; break;
			case DISPENSER_STOP:
            __NOP(); break;
			}
}

void Dispenser3_Process(void){
	if(Dispenser3state != prevDispenser3state){
			prevDispenser3state = Dispenser3state;
					if(Dispenser3state == DISPENSER_RUN){Dispenser3start();Dispenser3startTime = msCounter;}
					else{Dispenser3stop();}}
	switch(Dispenser3state)
			{case DISPENSER_RUN:
            if (msCounter >= Dispenser3startTime + Dispenser3targetTime) Dispenser3state = DISPENSER_STOP; break;
			case DISPENSER_STOP:
            __NOP(); break;
			}
}





void ManipulatorGetFromTable(void){
	__NOP();
}
void ManipulatorGetFromConveyor(void){
	__NOP();
}




void init(void){
		//BUTTON_PB6_Init();
		//Table_Init();
		BUTTON_PB10_Init();
		SysClock_init();
		Engine_A2A3_init();
		USART1_init();
		Engine_B6B7_init();
		Engine_B6B7_setPWM(0);
		Dispenser_A6_Init();
		Table_B4567_Init();
		prevtime = msCounter;
		Start_table_B4567();
		B12131415_Init();
		//Dispenser_A7_Init();
		//Dispenser_B0_Init();
		//StartTable();
		//Dispenser3start();
}
