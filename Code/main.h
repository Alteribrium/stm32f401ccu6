#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stm32f401xc.h>
#include "SysClock/SysClock.h"
#include "Engine/Engine_A2A3.h"
#include "Engine/Tahometer_B0B1.h"
#include "Engine/ControlSystem_A2A3.h"
#include "Engine/JustPWM_A2A3.h"
#include "WorkingFunctions.h"
#include "PB10/PB10.h"
//#include "USART/USART.h"
#include "modbus.h"
#include "USART/USART1.h"
#include "modbusFunctions.h"
#include "PA6/PA6.h"
#include "PB0/PB0.h"
#include "PB1/PB1.h"
#include "PA7/PA7.h"
#include "PB4567/PB4567.h"
#include "PB121314/PB121314.h"
#include "PC131415/PC131415.h"
#include "PB8/PB8.h"

typedef enum {
    Stop = 0,
    Start = 1,
		Complete = 2
} MainState_t;

extern uint8_t targetCounter;
extern uint8_t target;
extern MainState_t MainState;

#endif
