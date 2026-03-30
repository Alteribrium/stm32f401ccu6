#include "Engine/JustPWM_A2A3.h"

void SetPWM_A2A3(float newVoltage){
	if (ControlSystemStatus){
		Engine_A2A3_setPWM(newVoltage);
	}
	else{
		Engine_A2A3_setPWM(0);
	}
}
