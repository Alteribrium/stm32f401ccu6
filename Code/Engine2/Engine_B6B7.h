#ifndef Engine_B6B7_H_
#define Engine_B6B7_H_

#include "main.h"

#define MAXVOLTAGE_Engige_B6B7 24 //MAX VOLTAGE ON Engine Supply
#define MAXSPEEDB6B7 5600 //engine max speed on idle

void Engine_B6B7_init(void);
void Engine_B6B7_setPWM(float voltage);


#endif
