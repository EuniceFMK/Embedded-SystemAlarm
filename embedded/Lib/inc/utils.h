#include "stm32g0b1xx.h"

void Delay(void);
void Delay1ms(void);
void DelayX(int);



extern const char *termCMD[];

typedef enum termCMD_Index{
 CLEAR_SCREEN,
 CURSOR_HOME,
 CURSOR_LEFT,
 CURSOR_RIGHT,
 BGND_BLUE,
 FGND_RED
}CMD_Index;

uint32_t ConvertCharToInt(char*);

void ToBinary32(uint32_t , char *);