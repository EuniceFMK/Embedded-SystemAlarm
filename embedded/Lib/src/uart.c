#include "uart.h"
#include "gpio.h"
#include "utils.h"
#include <stdio.h>
void UART_Init(USART_TypeDef *pUart, uint32_t baud_rate, char inter)
{
    SystemCoreClockUpdate();
    // RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    // RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    if (pUart == USART2)
    {
        RCC->APBENR1 |= RCC_APBENR1_USART2EN;
        // GPIO_InitAlternateF(GPIOA, 2, 1);
        // GPIO_InitAlternateF(GPIOA, 3, 1);
    }

    if (pUart == USART1)
    {
        RCC->APBENR2 |= RCC_APBENR2_USART1EN;
        // GPIO_InitAlternateF(GPIOA, 9, 1);
        // GPIO_InitAlternateF(GPIOA, 10, 1);
    }

    pUart->CR1 &= ~USART_CR1_UE;
    pUart->CR1 |= USART_CR1_TE | USART_CR1_RE;
    pUart->BRR = SystemCoreClock / baud_rate;

    pUart->CR1 |= USART_CR1_UE;

    if(inter){
        pUart->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
       
        if(pUart == USART2)
            NVIC_EnableIRQ(USART2_LPUART2_IRQn);
        else if(pUart == USART1)
            NVIC_EnableIRQ(USART1_IRQn);
    }
    /*Some steps here might no necessarily be placed inside this function*/
    // Ensure that the port clock is turned on for GPIO
    // Ensure that the peripheral clock is enabled for USART2 (RCC_APBENR1 (RM::5.4.15))
    // Set pins TX and RX pins to the proper alternate function
    // Turn on the transmitter and receiver for the USART (USART_CR1 (RM::33.8.1))
    // Set the BAUD rate for the UART, using 10x fixed point arithmetic conversion with the known bus rate and
    // the desired BAUD rate in the calculation. (USART_BRR (RM::33.8.5))
    // Turn on the USART (USART_CR1 (RM::33.8.1)) (must be done after configuration)
}

void UART_TxByte(USART_TypeDef *pUart, uint8_t data)
{
    while (!(pUart->ISR & USART_ISR_TXE_TXFNF))
        ; // wait until data can be written into the buffer
    pUart->TDR = data;
    // Wait for the TXFNF flag of USART_ISR to set (USART_ISR (RM::33.8.9)).
    // Note: the TXFNF register name is odd in the derivative file!
    // Write the data to USART_TDR (USART_TDR (RM::33.8.13))
}

uint8_t UART_RxByte(USART_TypeDef *pUart, uint8_t *pData)
{
    if (pUart->ISR & USART_ISR_RXNE_RXFNE)
    {
        *pData = (uint8_t)pUart->RDR;
        return 1;
    }
    return 0;
    // If the RXFNE flag is set in USART_ISR, read USART_RDR and place the result at the target of the provided
    // data pointer (pData). Return a value indicating that the data target is valid. Note: the RXFNE register
    // name is odd in the derivative file! Otherwise, if the RXFNE flag is clear in USART_ISR,
    //  return a value indicating that the data target is not valid.
}
int UART_RxString(USART_TypeDef *pUSART, unsigned char *pTargetBuffer, unsigned short iBufferLength, _USART_RX_ENFORCE EnforceType)
{
    int index = 0;
    uint8_t c = 0;
    if (pTargetBuffer == NULL || iBufferLength < 1)
        return 0;
    while (index < iBufferLength - 1)
    {
        while (!UART_RxByte(pUSART, &c)) ; // Wait until we received a character from tthe terminal

            // If the character is back space
            if (c == '\b' || c == 0x7F)
            {
                if (index > 0)
                {
                    index--;
                    UART_TxByte(pUSART, '\b');
                    UART_TxByte(pUSART, ' ');
                    UART_TxByte(pUSART, '\b');
                }
                continue;
            }

        // Gestion ENTER
        if (c == '\r' || c == '\n')
        {
            pTargetBuffer[index] = '\0';
            UART_TxByte(pUSART, '\r');
            UART_TxByte(pUSART, '\n');
            return index;
        }

        if (EnforceType == RX_ONLY_ALPHA && (c < 65 || (c > 90 && c < 97) || c > 122))
            continue;

        if (EnforceType == RX_ONLY_DIGIT && (c < 48 || c > 57))
            continue;

        if (EnforceType == RX_ALNUM && ((c < 48) || (c > 57 && c < 65) || (c > 90 && c < 97) || (c > 122)))
            continue;

        pTargetBuffer[index++] = c;
        UART_TxByte(pUSART, c);
    }

    pTargetBuffer[index] = '\0';
    return index;
    //   If the target buffer is NULL or the buffer length is < 1, return 0
    //   While the target index (starts at 0) is less than the buffer length - 1:
    //     Block waiting for a character from the terminal
    //     If the character is ‘backspace’ (consult ASCII table), backup the target index (if possible, and echo the backspace character to the terminal
    //     If the character is ‘enter’, set the current buffer target to NUL and return the target position
    //     Depending on the enforcement type, if the character is valid, set the current buffer target to the received character, echo it, and advance the target position.
    //     If the target position equals the buffer length - 1, set the target buffer at the target position to NUL (or just blindly set the last buffer position to NUL).
    //   Return the target position
}

void UART_TxBuffer(USART_TypeDef *pUart, uint8_t *pData, uint16_t size)
{

    for (int i = 0; i < size; i++)
    {
        while (!(pUart->ISR & USART_ISR_TXE_TXFNF))
            ; // wait until data can be written into the buffer
        pUart->TDR = pData[i];
    }

    // Transmits an array of "size" bytes starting from *pData
}

void UART_TxStr(USART_TypeDef *pUart, const char *pData)
{
    while (*pData)
    {
         // wait until data can be written into the buffer
            UART_TxByte(pUart, *pData);
            pData++;        
    }
    // Transmits a string starting from *pData until the end of the string (null character)
}

void UART_TxStrGoToXY(USART_TypeDef *pUart, const char *pData,int col,int row)
{
    TERM_GoToXY(pUart,col,row);
    while (*pData)
    {
        //while (!(pUart->ISR & USART_ISR_TXE_TXFNF))
            //; // wait until data can be written into the buffer
            
            UART_TxByte(pUart, *pData);
            pData++;        
    }
    // Transmits a string starting from *pData until the end of the string (null character)
}

void TERM_GoToXY(USART_TypeDef *pUart, int col, int row)
{
    char buffer[20];
    if (col < 1)
        col = 1;
    if (row < 1)
        row = 1;
    // char c = (char)col;
    // char r = (char)row;

    sprintf(buffer, "\x1b[%d;%dH", row, col);
    UART_TxStr(pUart, buffer);
}

void TERM_ClearScreen(USART_TypeDef *pUart)
{
    UART_TxStr(pUart, termCMD[CLEAR_SCREEN]);
    UART_TxStr(pUart, termCMD[CURSOR_HOME]);
    // Use the \x1b[2J and \x1b[H escape sequences to clear the screen and go home.
}
