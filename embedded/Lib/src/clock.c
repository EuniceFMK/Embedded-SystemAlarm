
#include "clock.h"

void Clock_InitPll(PllRange range)
{
    /*Calibrate HSI16*/
    // RCC->ICSCR &= ~RCC_ICSCR_HSITRIM_Msk; // Clear current trim
    // RCC->ICSCR |= 65 << 8;

    RCC->CR &= ~RCC_CR_PLLON; // Disable the PLL
    RCC->PLLCFGR = range | (0b10 << RCC_PLLCFGR_PLLSRC_Pos);
    RCC->CR |= RCC_CR_PLLON; // Enable Pll again
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;                                                                         // Wait until PLL is locked
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLQEN; // Enable PLL/PLLR output

    RCC->CFGR &= ~RCC_CFGR_SW_Msk;          // Clear the PLL switch clock
    RCC->CFGR |= (0b10 << RCC_CFGR_SW_Pos); // Set Pll as System Clock
}

void Clock_EnableOutput(MCO_Select MCO, MCO_Div div)
{

    switch (MCO)
    {
    case MCO_Sel_HSI16:
        RCC->CR |= RCC_CR_HSION;                   // Enable HSI16
        while ((RCC->CR & RCC_CR_HSIRDY) == 0);    // Wait ready
        break;

    case MCO_Sel_HSE:
        RCC->CR |= RCC_CR_HSEON;                   // Enable HSE
        while ((RCC->CR & RCC_CR_HSERDY) == 0);    // Wait ready
        break;

    case MCO_Sel_LSI:
        RCC->CSR |= RCC_CSR_LSION;                 // Enable LSI
        while ((RCC->CSR & RCC_CSR_LSIRDY) == 0);  // Wait ready
        break;

    case MCO_Sel_LSE:
        RCC->BDCR |= RCC_BDCR_LSEON;               // Enable LSE
        while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);// Wait ready
        break;
    case MCO_Sel_SYSCLK:
        // No clock to enable, SYSCLK is already running
        break;

    default:
        break;
    }

    RCC->CFGR &= ~(0b111 << 24); // Clear the MCO register
    RCC->CFGR |= MCO;            // Set the output register
    RCC->CFGR &= ~(0b111 << 28); // Clear the MCOPRE register
    RCC->CFGR |= div;            // Set the prescaler div
}