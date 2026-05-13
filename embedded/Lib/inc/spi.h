#ifndef SPI_H
#define SPI_H

#include "stm32g0b1xx.h"
#include <stdint.h>

typedef struct
{
    SPI_TypeDef *instance;
} SPI_Handle_t;

/* Initialization */
void SPI_Init(SPI_Handle_t *spi,
              SPI_TypeDef *instance);

/* Basic transmit functions */
void SPI_Transmit(SPI_Handle_t *spi,
                  uint8_t *txData,
                  uint32_t len);

void SPI_Receive(SPI_Handle_t *spi,
                 uint8_t *rxData,
                 uint32_t len);

void SPI_TransmitReceive(SPI_Handle_t *spi,
                         uint8_t *txData,
                         uint8_t *rxData,
                         uint32_t len);

#endif