//******************
// SPI LCD ST7735 Library
//
// CREATED: 3/11/2026  
//
// FILE: st7735.h
//

#ifndef ST7735_H
#define ST7735_H

#include "st7735Fonts.h"
#include "lcd_colours.h"
#include "spi.h"

#define ST7735_WIDTH 160
#define ST7735_HEIGHT 128

typedef struct LCD_Driver__
{
    TIM_TypeDef *timer;
    SPI_Handle_t spiHandle;
    GPIO_TypeDef *csPort;
    uint16_t csPin;
    GPIO_TypeDef *rstPort;
    uint16_t rstPin;
    GPIO_TypeDef *dcPort;
    uint16_t dcPin;
} LCD_Driver_t;

typedef enum LCD_Rotation__
{
    LCD_ROT_0_P1 = 0x00,
    LCD_ROT_0_P2 = 0xC0,
    LCD_ROT_90_CW = 0x60,
    LCD_ROT_180 = 0xC0,
    LCD_ROT_270_CW = 0xA0
} LCD_Rotation_t;

void LCD_ST7735_HAL_Init(LCD_Driver_t *lcd, SPI_Handle_t *spiHandle,
                         GPIO_TypeDef *cs_Port, uint16_t cs_Pin,
                         GPIO_TypeDef *rst_Port, uint16_t rst_Pin,
                         GPIO_TypeDef *dc_Port, uint16_t dc_Pin);

void LCD_ST7735_Reset(LCD_Driver_t *lcd);
void LCD_ST7735_InitSequence(LCD_Driver_t *lcd, LCD_Rotation_t rot, TIM_TypeDef *_timer);
void LCD_ST7735_SetAddressWindow(LCD_Driver_t *lcd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void LCD_ST7735_FillScreen(LCD_Driver_t *lcd, uint16_t color);

void LCD_ST7735_DrawPixel(LCD_Driver_t *lcd, uint8_t x, uint8_t y, uint16_t color);
void LCD_ST7735_DrawChar(LCD_Driver_t *lcd, uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bg);
void LCD_ST7735_DrawString(LCD_Driver_t *lcd, uint8_t x, uint8_t y, const char *str, uint16_t color, uint16_t bg);

void LCD_ST7735_Cmd(LCD_Driver_t *lcd, uint8_t c);
void LCD_ST7735_CmdBuf(LCD_Driver_t *lcd, uint8_t *cmdBuf, uint8_t *rxDataBuf, uint32_t len);
void LCD_ST7735_Data(LCD_Driver_t *lcd, uint8_t d);
void LCD_ST7735_DataBuf(LCD_Driver_t *lcd, uint8_t *txDataBuf, uint8_t *rxDataBuf, uint32_t len);

#endif /* ST7735_H *//