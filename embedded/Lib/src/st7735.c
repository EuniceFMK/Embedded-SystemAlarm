#include "st7735.h"
#include "spi.h"
#include "gpio.h"
#include "timer.h"
#include "utils.h"
#include <stddef.h>

/*
Pinâ€‘byâ€‘Pin Meaning (ST7735S SPI Module)
GND: Ground
VCC: Power Supply (3.3V)
SCL: Serial Clock (SPI CLK)
SDA: Serial Data (SPI MOSI)
RES: Reset (Active Low)
DC: Data/Command Control (High for Data, Low for Command)
CS: Chip Select (Active Low)
BLK: Backlight Control (Optional - 3.3V PWM for brightness control)
*/

/***********Local functions Prototypes*******************/
void LCD_ST7735_Cmd(LCD_Driver_t *lcd, uint8_t c);
void LCD_ST7735_CmdBuf(LCD_Driver_t *lcd, uint8_t *cmdBuf, uint8_t *rxDataBuf, uint32_t len);
void LCD_ST7735_Data(LCD_Driver_t *lcd, uint8_t d);
void LCD_ST7735_DataBuf(LCD_Driver_t *lcd, uint8_t *txDataBuf, uint8_t *rxDataBuf, uint32_t len);
//*************************************************************************************
// PUBLIC FUNCTION IMPLEMENTATIONS
//*************************************************************************************

void LCD_ST7735_HAL_Init(LCD_Driver_t *lcd, SPI_Handle_t *spi,
                         GPIO_TypeDef *cs_Port, uint16_t cs_Pin,
                         GPIO_TypeDef *rst_Port, uint16_t rst_Pin,
                         GPIO_TypeDef *dc_Port, uint16_t dc_Pin)
{
    lcd->spiHandle = *spi;
    lcd->csPort = cs_Port;
    lcd->csPin = cs_Pin;
    lcd->rstPort = rst_Port;
    lcd->dcPort = dc_Port;
    lcd->rstPin = rst_Pin;
    lcd->dcPin = dc_Pin;

    GPIO_InitOutput(cs_Port, cs_Pin);
    GPIO_InitOutput(rst_Port, rst_Pin);
    GPIO_InitOutput(dc_Port, dc_Pin);
    GPIO_Set(cs_Port, cs_Pin);
    GPIO_Set(rst_Port, rst_Pin);
}

void LCD_ST7735_Reset(LCD_Driver_t *lcd)
{
    // 1. Pull the RST pin LOW.
    // 2. Delay for 10ms (Use your Timer_Delay_ms function).
    // 3. Pull the RST pin HIGH.
    GPIO_Clear(lcd->rstPort, lcd->rstPin);
    DelayX(100);
    GPIO_Set(lcd->rstPort, lcd->rstPin);
}

void LCD_ST7735_InitSequence(LCD_Driver_t *lcd, LCD_Rotation_t rot, TIM_TypeDef *_timer)
{
    // 1. Assign the timer to the lcd struct and initialize the ticker.
    // 2. Execute a Hardware Reset using LCD_ST7735_Reset().
    // 3. Delay 120ms.
    // 4. Send Command 0x11 (Sleep Out).
    // 5. Delay 120ms.
    // 6. Send Command 0x36 (Memory Data Access Control), then Data 'rot'.
    // 7. Send Command 0x3A (Interface Pixel Format), then Data 0x05 (16-bit color).
    // 8. Send Command 0x29 (Display ON).
    LCD_ST7735_Reset(lcd);
    DelayX(120);
    LCD_ST7735_Cmd(lcd, 0x11);
    DelayX(120);
    LCD_ST7735_Cmd(lcd, 0x36);
    LCD_ST7735_Data(lcd, rot);
    LCD_ST7735_Cmd(lcd, 0x3A);
    LCD_ST7735_Data(lcd, 0x05);
    LCD_ST7735_Cmd(lcd, 0x29);
}

void LCD_ST7735_SetAddressWindow(LCD_Driver_t *lcd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // 1. Column Address Set
    LCD_ST7735_Cmd(lcd, 0x2A);

    uint8_t colData[4];
    colData[0] = 0x00;
    colData[1] = x0;
    colData[2] = 0x00;
    colData[3] = x1;

    LCD_ST7735_DataBuf(lcd, colData, NULL, 4);

    // 2. Row Address Set
    LCD_ST7735_Cmd(lcd, 0x2B);

    uint8_t rowData[4];
    rowData[0] = 0x00;
    rowData[1] = y0;
    rowData[2] = 0x00;
    rowData[3] = y1;

    LCD_ST7735_DataBuf(lcd, rowData, NULL, 4);

    // 3. Memory Write
    LCD_ST7735_Cmd(lcd, 0x2C);
}

void LCD_ST7735_FillScreen(LCD_Driver_t *lcd, uint16_t color)
{
    // 1. Full screen window
    LCD_ST7735_SetAddressWindow(lcd, 0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);

    // 2. Prepare color buffer (RGB565)
    uint8_t buf[2];
    buf[0] = color >> 8;   // High byte
    buf[1] = color & 0xFF; // Low byte

    // 3. Total pixels
    uint32_t totalPixels = ST7735_WIDTH * ST7735_HEIGHT;

    // 4. Send color for each pixel
    for (uint32_t i = 0; i < totalPixels; i++)
    {
        LCD_ST7735_DataBuf(lcd, buf, NULL, 2);
    }
}

void LCD_ST7735_DrawPixel(LCD_Driver_t *lcd, uint8_t x, uint8_t y, uint16_t color)
{
    // 1. Set the address window to a single pixel at coordinates (x, y).
    LCD_ST7735_SetAddressWindow(lcd, x, y, x, y);

    // 2. Split the 16-bit color into a 2-byte buffer (High byte then Low byte).
    uint8_t buf[2];
    buf[0] = color >> 8;   // High byte
    buf[1] = color & 0xFF; // Low byte

    // 3. Send that 2-byte buffer using LCD_ST7735_DataBuf.
    LCD_ST7735_DataBuf(lcd, buf, NULL, 2);
}

void LCD_ST7735_DrawChar(LCD_Driver_t *lcd, uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bg)
{
    // 1. BOUNDARY CHECK:
    // The font array only contains characters from ASCII 32 (' ') to ASCII 127 ('~').
    // If 'c' is outside this range, change 'c' to a default character (e.g., '?').
    if (c < 32 || c > 127)
    {
        c = '?';
    }
    // 2. GET THE FONT DATA:
    // Create a pointer to the specific character's 5-byte array.
    // Example: const uint8_t *bitmap = font5x7[c - 32];
    // (We subtract 32 because ASCII 32 is at index 0 in our array).
    const uint8_t *bitmap = font5x7[c - 32];

    // 3. OUTER LOOP: Iterate through the 5 columns (Width)
    // Create a 'for' loop using a variable 'col' from 0 to 4.
    // {
    // a. Grab the current column's byte from the bitmap:
    //    uint8_t line = bitmap[col];

    // b. INNER LOOP: Iterate through the 8 bits of this column (Height)
    //    Create a 'for' loop using a variable 'row' from 0 to 7.
    //    {
    // i. Check if the lowest bit is a 1:  if (line & 0x01)
    //    - If TRUE: Call LCD_ST7735_DrawPixel at (x + col, y + row) with 'color'.
    //    - If FALSE: Call LCD_ST7735_DrawPixel at (x + col, y + row) with 'bg'.

    // ii. Shift the 'line' variable right by 1 bit (line >>= 1)
    //     so the next bit is ready for the next iteration of the inner loop.
    //    }
    // }
    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t line = bitmap[col];

        // 4. Loop through rows (bits)
        for (uint8_t row = 0; row < 8; row++)
        {
            if (line & 0x01)
            {
                // Pixel ON
                LCD_ST7735_DrawPixel(lcd, x + col, y + row, color);
            }
            else
            {
                // Pixel OFF (background)
                LCD_ST7735_DrawPixel(lcd, x + col, y + row, bg);
            }
            // Shift to next bit
            line >>= 1;
        }
    }

    // 4. CHARACTER SPACING:
    // After the outer loop finishes, the character is drawn!
    // But we need a 1-pixel gap between this character and the next one.
    // Create one final loop from row = 0 to 7, and draw a vertical line of pixels
    // at x-coordinate (x + 5) using the background color 'bg'.
    for (uint8_t row = 0; row < 8; row++)
    {
        LCD_ST7735_DrawPixel(lcd, x + 5, y + row, bg);
    }
}

void LCD_ST7735_DrawString(LCD_Driver_t *lcd, uint8_t x, uint8_t y, const char *str, uint16_t color, uint16_t bg)
{
    // 1. While the current character in the string is not the null terminator ('\0'):
    // 2. Call LCD_ST7735_DrawChar at the current (x, y).
    // 3. Increment 'x' by 6 pixels (5 for char width + 1 for spacing).
    // 4. Move to the next character pointer in the string.
    while (*str)
    {
        LCD_ST7735_DrawChar(lcd, x, y, *str, color, bg);
        x += 6; // Move to the next character position
        str++;  // Move to the next character in the string
    }
}

/******************Private functions**********************/
void LCD_ST7735_Cmd(LCD_Driver_t *lcd, uint8_t c)
{
    // 1. Set the Data/Command (DC) pin LOW to indicate a Command is being sent.
    // 2. Set Chip Select (CS) LOW to talk to the LCD.
    // 3. Use your SPI driver to transmit the single byte 'c'.
    // 4. Set Chip Select (CS) HIGH to finish the communication.
    GPIO_Clear(lcd->dcPort, lcd->dcPin);
    GPIO_Clear(lcd->csPort, lcd->csPin);
    SPI_Transmit(&lcd->spiHandle, &c, 1);
    // // 8-bit data size
    // while (!(SPI2->SR & SPI_SR_TXE))
    //     ; // Wait until TX register is empty
    // *((__IO uint8_t *)(&SPI2->DR)) = c;
    // while (!(SPI2->SR & SPI_SR_RXNE))
    //     ; // Wait until RX register is not empty
    // // rxData = (__IO uint8_t *)(spi->DR);
    // volatile uint8_t dum = SPI2->DR;
    GPIO_Set(lcd->csPort, lcd->csPin);
}

//-------------------------------------------------------------------------------------
void LCD_ST7735_CmdBuf(LCD_Driver_t *lcd, uint8_t *cmdBuf, uint8_t *rxDataBuf, uint32_t len)
{
    // 1. Set the Data/Command (DC) pin LOW to indicate a Command is being sent.
    // 2. Set Chip Select (CS) LOW to talk to the LCD.
    // 3. Use your SPI driver's "TransmitReceive" function to send the 'txDataBuf'  of length 'len'.
    // 4. Set Chip Select (CS) HIGH to finish the communication.
    GPIO_Clear(lcd->dcPort, lcd->dcPin);
    GPIO_Clear(lcd->csPort, lcd->csPin);
    SPI_TransmitReceive(&lcd->spiHandle,
                        cmdBuf,
                        rxDataBuf,
                        len);

    GPIO_Set(lcd->csPort, lcd->csPin);
}
//-------------------------------------------------------------------------------------
void LCD_ST7735_Data(LCD_Driver_t *lcd, uint8_t d)
{
    // 1. Set the Data/Command (DC) pin HIGH to indicate Data is being sent.
    // 2. Set Chip Select (CS) LOW to talk to the LCD.
    // 3. Use your SPI driver to transmit the single byte 'd'.
    // 4. Set Chip Select (CS) HIGH to finish the communication.

    GPIO_Set(lcd->dcPort, lcd->dcPin);
    GPIO_Clear(lcd->csPort, lcd->csPin);
    SPI_Transmit(&lcd->spiHandle, &d, 1);
    GPIO_Set(lcd->csPort, lcd->csPin);
}
//-------------------------------------------------------------------------------------
void LCD_ST7735_DataBuf(LCD_Driver_t *lcd, uint8_t *txDataBuf, uint8_t *rxDataBuf, uint32_t len)
{
    // 1. Set the Data/Command (DC) pin HIGH.
    // 2. Set Chip Select (CS) LOW.
    // 3. Use your SPI driver's "TransmitReceive" function to send the 'txDataBuf'  of length 'len'.
    // 4. Set Chip Select (CS) HIGH.

    GPIO_Set(lcd->dcPort, lcd->dcPin);
    GPIO_Clear(lcd->csPort, lcd->csPin);
    SPI_TransmitReceive(&lcd->spiHandle,
                        txDataBuf,
                        rxDataBuf,
                        len);
    GPIO_Set(lcd->csPort, lcd->csPin);
}

//-------------------------------------------------------------------------------------
