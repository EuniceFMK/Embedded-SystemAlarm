#ifndef UI_H
#define UI_H

#include "st7735.h"
#include "state_machine.h"

void UI_Init(LCD_Driver_t *lcd);
void UI_Update(SystemState state);

#endif