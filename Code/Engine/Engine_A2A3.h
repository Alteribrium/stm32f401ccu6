#ifndef Engine_A2A3_H_
#define Engine_A2A3_H_

#include "main.h"

#define MAXVOLTAGE_Engige_A2A3 24 //MAX VOLTAGE ON Engine Supply
#define MAXSPEEDA2A3 5600 //engine max speed on idle

void Engine_A2A3_init(void);
void Engine_A2A3_setPWM(float voltage);


#endif
