#ifndef WORKFUNC_H_
#define WORKFUNC_H_
#include "../main.h"

#define DEFAULT_CONVEYOR_VOLTAGE 12

void StartConveyor(void);
void StopConveyor(void);
void StopTable(void);
void StartTable(void);

void Dispenser1start(void);
void Dispenser2start(void);
void Dispenser3start(void);

void Dispenser1stop(void);
void Dispenser2stop(void);
void Dispenser3stop(void);
#endif
