#include "exti.h"

void EXTI_Init(GPIO_TypeDef* GPIOx, uint16_t pin, EXTI_Trigger trigger)
{
    // Determine the port index for the given GPIO port
    uint8_t portIndex;
    if (GPIOx == GPIOA) {
        portIndex = 0;
    } else if (GPIOx == GPIOB) {
        portIndex = 1;
    } else if (GPIOx == GPIOC) {
        portIndex = 2;
    } else if (GPIOx == GPIOD) {
        portIndex = 3;
    } else if (GPIOx == GPIOE) {
        portIndex = 4;
    } else if (GPIOx == GPIOF) {
        portIndex = 5;
    } else {
        return; // Invalid GPIO port
    }

    // Configure the EXTI line for the specified pin and trigger
    uint8_t pinNumber = __builtin_ctz(pin); // Get the pin number from the pin mask
    EXTI->EXTICR[pinNumber / 4] &= ~(0xF << ((pinNumber % 4) * 4)); // Clear existing settings
    EXTI->EXTICR[pinNumber / 4] |= (portIndex << ((pinNumber % 4) * 4)); // Set the port index

    // Configure the trigger type
    if (trigger & EXTI_Trigger_Rising) {
        EXTI->RTSR1 |= (1 << pinNumber); // Enable rising edge trigger
    } else {
        EXTI->RTSR1 &= ~(1 << pinNumber); // Disable rising edge trigger
    }

    if (trigger & EXTI_Trigger_Falling) {
        EXTI->FTSR1 |= (1 << pinNumber); // Enable falling edge trigger
    } else {
        EXTI->FTSR1 &= ~(1 << pinNumber); // Disable falling edge trigger
    }

    // Enable the interrupt for the specified pin
    EXTI->IMR1 |= (1 << pinNumber);


    
//   EXTI->EXTICR[2] &= ~EXTI_EXTICR3_EXTI8;  // Clear EXTI8 settings
//   EXTI->EXTICR[2] |= EXTI_EXTICR3_EXTI8_1; // Select PD 0b1100
//   EXTI->RTSR1 &= ~EXTI_RTSR1_RT8;          // Disable rising
//   EXTI->FTSR1 |= EXTI_FTSR1_FT8;           // Enable falling
//   EXTI->IMR1 |= EXTI_IMR1_IM8;             // Enable interrupt
}