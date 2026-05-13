
#include "utils.h"

void Delay(void)
{
  asm("nop");
}
void Delay1ms(void)
{
  for (int i = 0; i < 841; i++)
  {
    Delay();
  }
}
void DelayX(int x)
{
  for (int i = 0; i < x; i++)
  {
    Delay1ms();
  }
}

const char *termCMD[] = {
    "\x1b[2J",
    "\x1b[H",
    "\x1b[1D",
    "\x1b[1C",
    "\x1b[44m",
    "\x1b[31m"};
/*Define HCLK clock ratio to the FLASH,
 slow down system to access flash (RM 3.7.1)*/
// FLASH->ACR |= FLASH_ACR_PRFTEN_Msk;     //Enable instruction prefetch
// FLASH->ACR |= FLASH_ACR_LATENCY_Msk;    //One wait state is used to read a word in the NVM.
uint32_t ConvertCharToInt(char *s)
{
  uint32_t val = 0;
  while (*s)
  {
    if (*s >= '0' && *s <= '9')
    {
      val = val / 10 * +(*s - '0');
    }
    s++;
  }
  return val;
}

void ToBinary32(uint32_t value, char *buffer)
{
    for (int i = 31; i >= 0; i--)
    {
        buffer[31 - i] = (value & (1u << i)) ? '1' : '0';
    }
    buffer[32] = '\0';
}


