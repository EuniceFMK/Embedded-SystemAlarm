
#include "keypad.h"
#include "gpio.h"
#include "timer.h"

/* =========================
   CONFIG (À ADAPTER À TON HARDWARE)
   ========================= */

#define ROWS 4
#define COLS 3  

/* EXEMPLE PINS (à adapter !) */
static uint8_t rowPins[ROWS] = {0, 1, 2, 3};   // GPIO rows
static uint8_t colPins[COLS] = {4, 5, 6};     // GPIO cols

static GPIO_TypeDef *ROW_PORT = GPIOD;
static GPIO_TypeDef *COL_PORT = GPIOC;