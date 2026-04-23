#include "WorkingFunctions.h"
#include <stdio.h>
#include <string.h>


//conveyor
void StartConveyor(void){
	Engine_A2A3_setPWM(DEFAULT_CONVEYOR_VOLTAGE);
}

void StopConveyor(void){
	Engine_A2A3_setPWM(0);
}




//table
void StopTable(void){
	Stop_Table();
}

void StartTable(void){
	Start_Table();
}


void Dispenser1start(void){
	Dispenser1state = 1;
}
void Dispenser2start(void){
	Dispenser2state = 1;
}
void Dispenser3start(void){
	Dispenser3state = 1;
}