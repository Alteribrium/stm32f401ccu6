#ifndef ControlSystem_A2A3_H_
#define ControlSystem_A2A3_H_

#include "main.h"



void ControlSystem_A2A3_init(void);
void TIM4_IRQHandler(void);
void ControlSystemA2A3(void);

extern uint8_t Flag_ControlSysetmA2A3;
extern int32_t targetSpeedA2A3;
extern float dif;
extern float voltagediff;

extern int32_t position;

#endif
