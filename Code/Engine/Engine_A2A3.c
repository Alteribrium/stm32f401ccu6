#include "Engine/Engine_A2A3.h" // ???????????, ??? ??? ????? ?? ??

void Engine_A2A3_init(void);
void Engine_A2A3_setPWM(float voltage);

void Engine_A2A3_init(void){

    //Tahometer_B0B1_init();
    //ControlSystem_A2A3_init(); //For tahoemter
		
	
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    GPIOA->MODER &= ~GPIO_MODER_MODER0;  
    GPIOA->MODER |= GPIO_MODER_MODER0_1;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;  
    GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL0_Pos);

    GPIOA->MODER &= ~GPIO_MODER_MODER1; 
    GPIOA->MODER |= GPIO_MODER_MODER1_1;  
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL1;  
    GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL1_Pos);

    TIM5->PSC = 1600 - 1;
    TIM5->ARR = 100;


    TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM5->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM5->CCMR1 |= TIM_CCMR1_OC1PE;

    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM5->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos); 
    TIM5->CCMR1 |= TIM_CCMR1_OC2PE;    

    TIM5->CCER |= TIM_CCER_CC1E;
    TIM5->CCER |= TIM_CCER_CC2E;

    TIM5->EGR |= TIM_EGR_UG;
    TIM5->CR1 |= TIM_CR1_CEN;

    TIM5->CCR1 = 0;
    TIM5->CCR2 = 0;
}

void Engine_A2A3_setPWM(float voltage){
    int16_t PWM = (int16_t)(voltage * 100 / MAXVOLTAGE_Engige_A2A3);
    if(voltage >= 0){
        TIM5->CCR1 = (uint32_t)(PWM);
        TIM5->CCR2 = 0;
    }
    else{
        TIM5->CCR1 = 0;
        TIM5->CCR2 = (uint32_t)((-1) * PWM);
    }
}
