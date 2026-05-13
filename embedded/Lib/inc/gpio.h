//******************
//GPIO Library
// General purpose Input Output Library
// CREATED: 10/27/2023, by Carlos Estay
//
// FILE: gpio.h
//


#ifndef GPIO_H
#define GPIO_H

//requires 
#include "stm32g0b1xx.h"
extern uint32_t SystemCoreClock;

// type definitions go here
 typedef enum GPIO_IO_PullModeTypedef__
    {
        PullMode_None = 0b00,
        PullMode_PullUp = 0b01,
        PullMode_PullDown = 0b10,
        PullMode_Mask = 0b11
    }IO_PullMode;

    typedef enum GPIO_IO_TypeTypedef__
    {
        Type_PushPull = 0,
        Type_OpenDrain = 1,
    }IO_Type;
//Set the mode of an individual Pin

  /******Prototypes******/

  /// @brief Set GPIO pin as output
  /// @param  GPIO Port
  /// @param GPIO pin 
  void GPIO_InitInput(GPIO_TypeDef*, uint16_t);

  /// @brief Set GPIO pin as input
  /// @param  GPIO Port
  /// @param GPIO pin  
  void GPIO_InitOutput(GPIO_TypeDef*, uint16_t);

  /// @brief Set bit in Port
  /// @param Port 
  /// @param pin  
  void GPIO_Set(GPIO_TypeDef*, uint16_t);
  
  /// @brief Clear bit in Port
  /// @param Port 
  /// @param pin  
  void GPIO_Clear(GPIO_TypeDef*, uint16_t);
  
  /// @brief Toggle bit in Port
  /// @param Port 
  /// @param pin   
  void GPIO_Toggle(GPIO_TypeDef* , uint16_t);
  
  /// @brief Reads pin in Port
  /// @param Port 
  /// @param pin       
  int GPIO_Read(GPIO_TypeDef*, uint16_t);

  /// @brief Set the pins to the alternate function
  /// @param  Port
  /// @param  pin
  /// @param  
  void GPIO_InitAlternateF(GPIO_TypeDef*, uint16_t,uint16_t);
  void GPIO_SetPullMode(GPIO_TypeDef *, uint16_t , IO_PullMode );
 
  void GPIO_SetPullUp(GPIO_TypeDef*, uint16_t);
  //Pin modes

  typedef enum SpeedTypedef__
{
    Speed_VeryLow = 0b00,
    Speed_Low = 0b01,
    Speed_High = 0b10,
    Speed_VeryHigh = 0b11
} Speed;
void GPIO_SetSpeed(GPIO_TypeDef *, uint16_t, Speed);
#endif /* GPIO_H */