//******************
// Clock Library
//
// CREATED: Sept/24/2024, by Carlos Estay
//
// FILE: clock.h
//
//
#include "stm32g0b1xx.h"

#ifndef CLOCK_H
#define CLOCK_H

#define RCC_CFGR_SW_PLL RCC_CFGR_SW_1 // PLL as system clock
/*
  PLL_CLK = PLL_IN x (N / M) / R
  Minumum R = 2

  R and M factor must subtract 1 as 0 means setting 1
*/
typedef enum PllRangeTypedef__
{                                                                                                               //(R-1)<<RCC_PLLCFGR_PLLR_Pos |  N<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos  // PLL_IN   N   M   R
    PLL_16MHZ = (8 - 1) << RCC_PLLCFGR_PLLR_Pos | 8 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos,  // 16MHz x  8 / 1 / 8 = 16MHz
    PLL_20MHZ = (4 - 1) << RCC_PLLCFGR_PLLR_Pos | 5 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos,  // 16MHz x  8 / 1 / 8 = 16MHz,
    PLL_32MHZ = (8 - 1) << RCC_PLLCFGR_PLLR_Pos | 16 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz,
    PLL_40MHZ = (4 - 1) << RCC_PLLCFGR_PLLR_Pos | 10 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz,,
    PLL_48MHZ = (7 - 1) << RCC_PLLCFGR_PLLR_Pos | 15 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz,,
    PLL_50MHZ = (8 - 1) << RCC_PLLCFGR_PLLR_Pos | 25 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz,,
    PLL_60MHZ = (2 - 1) << RCC_PLLCFGR_PLLR_Pos | 30 << RCC_PLLCFGR_PLLN_Pos | (4 - 1) << RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz,,
    PLL_64MHZ = (4 - 1) << RCC_PLLCFGR_PLLR_Pos | 16 << RCC_PLLCFGR_PLLN_Pos | (1 - 1) << RCC_PLLCFGR_PLLM_Pos  // 16MHz x  8 / 1 / 8 = 16MHz,,

} PllRange;

typedef enum MCO_DivTpedef__
{
    MCO_Div1 = 0U << 28,
    MCO_Div2 = 1U << 28,
    MCO_Div4 = 2U << 28,
    MCO_Div8 = 3U << 28,
    MCO_Div16 = 4U << 28,
    MCO_Div32 = 5U << 28,
    MCO_Div64 = 6U << 28
} MCO_Div;

typedef enum MCO_SelectTpedef__
{
    MCO_Sel_None = 0U << 24,
    MCO_Sel_SYSCLK = 1U << 24,
    MCO_Sel_HSI16 = 3U << 24,
    MCO_Sel_HSE = 4U << 24,
    MCO_Sel_PLL = 5U << 24,
    MCO_Sel_LSI = 6U << 24,
    MCO_Sel_LSE = 7U << 24
} MCO_Select;

void Clock_InitPll(PllRange);
void Clock_EnableOutput(MCO_Select, MCO_Div);

#endif /* CLOCK_H */