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

//Dispensers
void Dispenser1start(void){
	Start_Dispenser_A4();
}

void Dispenser1stop(void){
	Stop_Dispenser_A4();
}

void Dispenser2start(void){
	Start_Dispenser_A5();
}

void Dispenser2stop(void){
	Stop_Dispenser_A5();
}

void Dispenser3start(void){
	Start_Dispenser_A6();
}

void Dispenser3stop(void){
	Stop_Dispenser_A6();
}
