#include "modbusFunctions.h"
#include <stdio.h>
#include <string.h>

void WriteModbus(void){
	ControlSystemStatus = OutputRegisters[CONTROL_SYSTEM_STATUS_ADDRESS] & 1;
	targetSpeedA2A3 = OutputRegisters[1];
}
/*
void putFloatToAddress(float val, uint16_t addr) {
	uint32_t intVal;
	memcpy(&intaVal, &val, 4);
	
	InputRegister[addr] = intVal & 0xFFFF;
	InputRegister[addr+1] = (intVal & 0xFFFF0000) >> 16;
}


void putMotorSpeedToMobus(float val) {
	putFloatToAddress(val, MOTOR_SPEED_ADRESS);
}*/
