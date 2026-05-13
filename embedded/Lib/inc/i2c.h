//******************
//I2C Library
//
// CREATED: 01/28/2025 
//
// FILE: i2c.h
//



#ifndef I2C_H
#define I2C_H

#include "stm32g0b1xx.h"


/*
To use this enum properly configure the I2C to be clocked directly from HSI16
5.4.21 RCC_CCIPR

*/
typedef enum I2C_Speed_Typedef__
{
  I2C_Standard = 0x00503D58,    //100KHz
  I2C_Fast =     0x00300617,    //400KHz
  I2C_FastPlus = 0x00200105     //1MHz
}I2C_Speed;



void I2C_Init(I2C_TypeDef* i2c, I2C_Speed speed);
void I2C_Reset(I2C_TypeDef* i2c);
int I2C_IsBusy(I2C_TypeDef* i2c);
void  I2C_ScanFirstAddr(I2C_TypeDef* i2c);
int I2C_Transmit(I2C_TypeDef* i2c, uint8_t addr, uint8_t* pData, uint8_t size);
int I2C_Writereg(I2C_TypeDef* i2c, uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t size);
int I2C_Readreg(I2C_TypeDef* i2c, uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t size);






#endif /* I2C */