#include "WorkingFunctions.h"
#include <stdio.h>
#include <string.h>

void StartConveyor(void){
	Engine_A2A3_setPWM(DEFAULT_CONVEYOR_VOLTAGE);
}

void StopConveyor(void){
	Engine_A2A3_setPWM(0);
}
