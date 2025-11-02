#ifndef Tahometer_B0B1_H_
#define Tahometer_B0B1_H_

#include "main.h"

#define SpeedScaler 1200000 //time scaler 60/50*1000000 cus 60sec on min 500 encoder values reqire but 10 is used 1000000 ms on sec 

void Tahometer_B0B1_init(void);
void TIM3_IRQHandler(void);
void Tahometer_B0B1_riseDetected(uint32_t value,int8_t dir);
void Tahometer_B0B1_zeroriseDetected(void);
void TIM2_IRQHandler(void);


extern int32_t speedA2A3;


#endif
