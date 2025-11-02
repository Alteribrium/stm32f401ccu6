#include "Engine/ControlSystem_A2A3.h"

void ControlSystem_A2A3_init(void);
void TIM4_IRQHandler(void);
void ControlSystemA2A3(void);

uint8_t Flag_ControlSysetmA2A3 = 0;
int32_t targetSpeedA2A3 = 0;
float dif = 0;
float voltagediff = 0;
static float voltage = 0;

void ControlSystemA2A3(void){
	dif = (float)(targetSpeedA2A3 - speedA2A3);
	if (targetSpeedA2A3 == 0){
		voltagediff = 0;
		voltage = 0;
	}
	else if (dif < (float)(MAXSPEEDA2A3 *0.05) && dif > (float)(-MAXSPEEDA2A3 *0.05)){
		__NOP();
		voltagediff = 0;
	}
	else if (dif < (float)(MAXSPEEDA2A3 *0.2) && dif > (float)(-MAXSPEEDA2A3 *0.2)){
		voltagediff = (float)(dif* MAXVOLTAGE_Engige_A2A3 / MAXSPEEDA2A3 * (float)(0.5));
		voltage += (float)(dif * MAXVOLTAGE_Engige_A2A3 / MAXSPEEDA2A3 * (float)(0.5));
	}
	else{
		voltagediff  = (float)(MAXVOLTAGE_Engige_A2A3 * 0.15 * (dif > 0 ? 1 : -1) * 0.8);
		voltage += (float)(MAXVOLTAGE_Engige_A2A3 * 0.15 * (dif > 0 ? 1 : -1) * 0.8);
	}
	Engine_A2A3_setPWM(voltage);
}

void ControlSystem_A2A3_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC = 1600 - 1;
	TIM4->ARR = 1000;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_IRQHandler(void){
	if(TIM4->SR & TIM_SR_UIF){
		TIM4->SR &= ~TIM_SR_UIF;
		Flag_ControlSysetmA2A3 = 1;
	}
}
