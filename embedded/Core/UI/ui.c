
#include "ui.h"

static LCD_Driver_t *lcd_ref = NULL;

/* -----------------------------
 * INIT UI
 * ----------------------------- */
void UI_Init(LCD_Driver_t *lcd)
{
    lcd_ref = lcd;

    LCD_ST7735_FillScreen(lcd_ref, LCD_BLACK);

    LCD_ST7735_DrawString(lcd_ref,
                          10,
                          10,
                          "SMART SECURITY",
                          LCD_WHITE,
                          LCD_BLACK);
}

void UI_Update(SystemState state)
{
    switch(state)
    {
        case STATE_IDLE:

            LCD_ST7735_DrawString(lcd_ref,
                                  10,
                                  40,
                                  "STATUS: IDLE   ",
                                  LCD_GREEN,
                                  LCD_BLACK);
            break;

        case STATE_ARMED:

            LCD_ST7735_DrawString(lcd_ref,
                                  10,
                                  40,
                                  "STATUS: ARMED  ",
                                  LCD_YELLOW,
                                  LCD_BLACK);
            break;

        case STATE_ALARM:

            LCD_ST7735_DrawString(lcd_ref,
                                  10,
                                  40,
                                  "STATUS: ALARM! ",
                                  LCD_RED,
                                  LCD_BLACK);
            break;

        case STATE_ERROR:

            LCD_ST7735_DrawString(lcd_ref,
                                  10,
                                  40,
                                  "STATUS: ERROR  ",
                                  LCD_RED,
                                  LCD_BLACK);
            break;

        default:
            break;
    }
}