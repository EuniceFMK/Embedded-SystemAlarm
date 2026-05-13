#include "spi.h"
#include <stddef.h>
void SPI_Init(SPI_Handle_t *spi,
              SPI_TypeDef *instance)
{
    spi->instance = instance;

    /* Disable SPI before configuration */
    instance->CR1 &= ~SPI_CR1_SPE;

    /* Baud rate = fPCLK / 8 */
    instance->CR1 &= ~SPI_CR1_BR;
    instance->CR1 |= SPI_CR1_BR_1;

    /* SPI Mode 3 */
    instance->CR1 |= SPI_CR1_CPOL;
    instance->CR1 |= SPI_CR1_CPHA;

    /* Master mode */
    instance->CR1 |= SPI_CR1_MSTR;

    /* Software slave management */
    instance->CR1 |= SPI_CR1_SSM;
    instance->CR1 |= SPI_CR1_SSI;

    /* 8-bit data size */
    instance->CR2 |= SPI_CR2_DS_2 |
                     SPI_CR2_DS_1 |
                     SPI_CR2_DS_0;

    /* RXNE event on 8-bit */
    instance->CR2 |= SPI_CR2_FRXTH;

    /* Enable SPI */
    instance->CR1 |= SPI_CR1_SPE;
}

void SPI_Transmit(SPI_Handle_t *spi,
                  uint8_t *txData,
                  uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(spi->instance->SR & SPI_SR_TXE));

        *((__IO uint8_t*)&spi->instance->DR) = txData[i];

        while(!(spi->instance->SR & SPI_SR_RXNE));

        volatile uint8_t dummy =
            *((__IO uint8_t*)&spi->instance->DR);

        (void)dummy;
    }
}

void SPI_Receive(SPI_Handle_t *spi,
                 uint8_t *rxData,
                 uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(spi->instance->SR & SPI_SR_TXE));

        *((__IO uint8_t*)&spi->instance->DR) = 0xFF;

        while(!(spi->instance->SR & SPI_SR_RXNE));

        rxData[i] =
            *((__IO uint8_t*)&spi->instance->DR);
    }
}

void SPI_TransmitReceive(SPI_Handle_t *spi,
                         uint8_t *txData,
                         uint8_t *rxData,
                         uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(spi->instance->SR & SPI_SR_TXE));

        *((__IO uint8_t*)&spi->instance->DR) = txData[i];

        while(!(spi->instance->SR & SPI_SR_RXNE));

        uint8_t data =
            *((__IO uint8_t*)&spi->instance->DR);

        if(rxData != NULL)
        {
            rxData[i] = data;
        }
    }
}