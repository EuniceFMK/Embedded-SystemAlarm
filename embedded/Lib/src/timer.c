#include "timer.h"

void Timer_Init(TIM_TypeDef *timer, uint16_t psc, uint16_t period)
{

    // Disable timer, in case already enabled
    timer->CR1 &= ~TIM_CR1_CEN;

    // Set prescaler
    timer->PSC = psc - 1; // ca compte tous les psc cycles de la frequence a laquelle tu run

    // Set Autoreload in ARR register
    timer->ARR = period - 1; // period
    // timer->CCMR1 |= TIM_CCMR1_OC1PE;
    // timer->CR1 |= TIM_CR1_ARPE;
    // timer->EGR |= TIM_EGR_UG; // Generate an update event to reload the prescaler value immediately
}

void Timer_SetupChannel(TIM_TypeDef *timer, CCR_Typedef ccr, ChannelMode_Typedef chMode)
{
    uint32_t shift = 0;

    switch (chMode)
    {
    case InputCapture:
        timer->CCMR1 &= ~TIM_CCMR1_CC1S_Msk; // Clear capture-compare
        timer->CCMR1 |= TIM_CCMR1_CC1S_0;    // CC1 channel is configured as input, IC1 is mapped on TI1
        timer->CCER |= TIM_CCER_CC1E; // Enable channel 1 capture
    }
    switch (ccr)
    {
    case TimCCR1:
        shift = 4; // OC1M bits (6:4)
        timer->CCMR1 &= ~(0x7 << shift);
        timer->CCMR1 |= (chMode << shift);
        timer->CCER |= TIM_CCER_CC1E;
        break;

    case TimCCR2:
        shift = 12; // OC2M bits (14:12)
        timer->CCMR1 &= ~(0x7 << shift);
        timer->CCMR1 |= (chMode << shift);
        timer->CCER |= TIM_CCER_CC2E;
        break;

    case TimCCR3:
        shift = 4;
        timer->CCMR2 &= ~(0x7 << shift);
        timer->CCMR2 |= (chMode << shift);
        timer->CCER |= TIM_CCER_CC3E;
        break;

    case TimCCR4:
        shift = 12;
        timer->CCMR2 &= ~(0x7 << shift);
        timer->CCMR2 |= (chMode << shift);
        timer->CCER |= TIM_CCER_CC4E;
        break;
    }
}

void Timer_SetEnable(TIM_TypeDef *timer, uint16_t en)
{
    if (en)
        timer->CR1 |= TIM_CR1_CEN;
    else
        timer->CR1 &= ~TIM_CR1_CEN;
}

void Timer_EnableInterrupt(TIM_TypeDef *timer, IRQn_Type timerIRQn, Timer_IE interruptMask)
{
    timer->DIER |= interruptMask; // enable update or CCx interrupt
    NVIC_EnableIRQ(timerIRQn);
}

void Timer_WriteCCR(TIM_TypeDef *timer, CCR_Typedef ccr, uint32_t val)
{
    *(volatile uint32_t *)((uint32_t)timer + 0x34 + ccr) = val;
}
