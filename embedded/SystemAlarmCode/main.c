/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
File    : main.c
Purpose : Lab3
Author  : Eunice De Grace Fmukam Ngadjou
Description :
Date    : April-04-2026

*/

/*Default includes*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stm32g0b1xx.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "utils.h"
#include "timer.h"
#include "exti.h"
#include "st7735.h"

/*********************************************************************
 *
 *       main()
 *
 *  Function description
 *   Application entry point.
 */
/*Local Prototypes*/

// #define DMA

void HAL_Init(void);

int main(void)
{

  HAL_Init();
  Clock_InitPll(PLL_64MHZ);

  UART_Init(USART2, 9600, 0);
  /*Set Baud Rate*/
  SPI2->CR1 &= ~SPI_CR1_BR;  // Clear BR settings
  SPI2->CR1 |= SPI_CR1_BR_1; // DIvide by 8 -> 5[MHz]
  /*Polarity and Phase*/
  SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA; // Set mode to (1,1) (pol. pha)
  SPI2->CR1 |= SPI_CR1_MSTR;                // Master Configuration
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;   // Soft Slave mngment enabled
  // 8-bit data  size
  SPI2->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
  SPI2->CR2 |= SPI_CR2_FRXTH; // 8-bit data  event
  SPI2->CR1 |= SPI_CR1_SPE;   // Enable SPI

  GPIO_InitOutput(GPIOD, 5);
  GPIO_InitOutput(GPIOC, 8);
  GPIO_InitOutput(GPIOC, 6);
  GPIO_InitAlternateF(GPIOA, 12, 0);
  GPIO_InitAlternateF(GPIOB, 3, 0);

  LCD_Driver_t lcd;

  LCD_ST7735_HAL_Init(&lcd,
                      SPI2,
                      GPIOD, 5,  // CS
                      GPIOC, 8,  // RST
                      GPIOC, 6); // DC

  LCD_ST7735_FillScreen(&lcd, LCD_RED);

  LCD_ST7735_Reset(&lcd);

  LCD_ST7735_InitSequence(&lcd, LCD_ROT_0_P1, TIM2);

  while (1)
  {
   LCD_ST7735_FillScreen(&lcd, LCD_GREEN);
     DelayX(50000);

    LCD_ST7735_FillScreen(&lcd, LCD_BLUE);
     DelayX(50000);
  }
}
/********************************************************************/

void HAL_Init(void)
{
  // Enable power interface clock (RM 5.4.15)
  RCC->APBENR1 |= RCC_APBENR1_PWREN_Msk;

  /*Define HCLK clock ratio to the FLASH,
    slow down system to access flash (RM 3.7.1)*/

  FLASH->ACR |= FLASH_ACR_PRFTEN_Msk;  // Enable instruction prefetch
  FLASH->ACR |= FLASH_ACR_LATENCY_Msk; // One wait state is used to read a word in the NVM.

  /*Peripherals clock enable (RM 5.4.13)*/

  RCC->IOPENR |= RCC_IOPENR_GPIOAEN_Msk; // Enable Port A
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN_Msk; // Enable Port B
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN_Msk; // Enable Port C
  RCC->IOPENR |= RCC_IOPENR_GPIODEN_Msk; // Enable Port D

  RCC->APBENR1 |= RCC_APBENR1_USART2EN_Msk; // Enable USART2
  RCC->APBENR2 |= RCC_APBENR2_USART1EN_Msk; // Enable USART2
  RCC->APBENR1 |= RCC_APBENR1_SPI2EN;       // Enable SPI2
}

/*************************** End of file ****************************/
