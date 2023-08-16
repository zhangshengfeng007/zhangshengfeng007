#include "includes.h"

void GPIO_Init_Handle(void)
{
  RCC->AHBENR|=1<<17 ;   //??GPIOA??
}


