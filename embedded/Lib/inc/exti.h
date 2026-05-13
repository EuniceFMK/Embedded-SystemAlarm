
//  #ifndef EXTI_H
// #define EXTI_H

// requires
#include "stm32g0B1xx.h"

typedef enum EXTI_TriggerTypedef__
{
    EXTI_Trigger_Rising = 0b01,
    EXTI_Trigger_Falling = 0b10,
    EXTI_Trigger_RisingFalling = 0b11
    // EXTI_Trigger_RisingFalling = 0b11
} EXTI_Trigger;

void EXTI_Init(GPIO_TypeDef *, uint16_t, EXTI_Trigger);