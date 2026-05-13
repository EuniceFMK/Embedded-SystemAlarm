//******************
// GPIO Library
// General purpose Input Output Library
// CREATED: 10/27/2023, by Carlos Estay
//
// FILE: gpio.h
//
#include "gpio.h"
/// @brief Set GPIO pin as output
/// @param GPIO Port
/// @param GPIO pin
void GPIO_InitInput(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 && PinNumber <= 15)
   {
      pPort->MODER &= ~(0b11 << PinNumber * 2);
      pPort->MODER &= ~(0b11 << PinNumber * 2);
   }
}

/// @brief Set GPIO pin as input
/// @param GPIO Port
/// @param GPIO pin
void GPIO_InitOutput(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 || PinNumber <= 15)
   {
      pPort->MODER &= ~(0b11 << PinNumber * 2);
      pPort->MODER |= 0b01 << PinNumber * 2;
   }
}

/// @brief Set pull-up or pull-down
/// @param GPIO Port
/// @param GPIO pin
/// @param mode
void GPIO_SetPullMode(GPIO_TypeDef *pPort, uint16_t PinNumber, IO_PullMode mode)
{
   if (PinNumber >= 0 || PinNumber <= 15)
   {
      pPort->PUPDR &= ~(0b11 << (PinNumber * 2));
      pPort->PUPDR |= (mode << (PinNumber * 2));
   }
}

/// @brief Set type of Output: Push-Pull or Open Collector
/// @param GPIO Port
/// @param GPIO pin
/// @param type
void GPIO_SetType(GPIO_TypeDef *, uint16_t, IO_Type)
{
}

/// @brief Set alternate function (AFx MUX)
/// @param GPIO Port
/// @param GPIO pin
/// @param AF
void GPIO_InitAlternateF(GPIO_TypeDef *pPort, uint16_t PinNumber, uint16_t alternate)
{
   pPort->MODER &= ~(0b11 << (PinNumber * 2));
   pPort->MODER |= (0b10 << (PinNumber * 2));
   if (PinNumber < 8)
   {
      pPort->AFR[0] &= ~(0xF << (PinNumber * 4));
      pPort->AFR[0] |= (alternate << (PinNumber * 4));
   }
   else
   {
      pPort->AFR[1] &= ~(0xF << ((PinNumber - 8) * 4));
      pPort->AFR[1] |= (alternate << ((PinNumber - 8) * 4));
   }
}

/// @brief Set bit in Port
/// @param Port
/// @param pin
void GPIO_Set(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 && PinNumber <= 15)
   {
      pPort->ODR |= 0b1 << PinNumber;
   }
}

/// @brief Clear bit in Port
/// @param Port
/// @param pin
void GPIO_Clear(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 && PinNumber <= 15)
   {
      pPort->ODR &= ~(0b1 << PinNumber);
   }
}

/// @brief Toggle bit in Port
/// @param Port
/// @param pin
void GPIO_Toggle(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 && PinNumber <= 15)
   {
      pPort->ODR ^= 0b1 << PinNumber;
   }
}

/// @brief Reads pin in Port
/// @param Port
/// @param pin
int GPIO_Read(GPIO_TypeDef *pPort, uint16_t PinNumber)
{
   if (PinNumber >= 0 && PinNumber <= 15)
   {
      if ((pPort->IDR & (1 << PinNumber)) != 0)  // The pin is high
      {
         return 1; // return true
      }
      else
      {
         return 0;
      }
   }
}

void GPIO_SetSpeed(GPIO_TypeDef *pPort, uint16_t pinNumber, Speed sp)
{
  pPort->OSPEEDR &= ~(3U << (pinNumber * 2)); // Clear the speed register
  pPort->OSPEEDR |= (sp << (pinNumber * 2));  // Set the speed
}