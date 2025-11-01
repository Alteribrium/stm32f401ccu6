#ifndef Engine_A2A3_H_
#define Engine_A2A3_H_

#include "main.h"

#define MAXVOLTAGE_Engige_A2A3 24 //MAX VOLTAGE ON Engine Supply

void Engine_A2A3_init(void);
void Engine_A2A3_setPWM(float voltage);

#endif
