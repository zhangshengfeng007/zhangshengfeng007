#include "Delay.h"

//=====================
void Delay(uint32_t count);

void Delay_MS(uint32_t count);

void Delay(uint32_t count)
{
    for(; count > 0; count--)
        ;
}

void Delay_MS(uint32_t count)
{
    uint16_t Delay_MS_time;

    for(; count > 0; count--)
    {
        Delay_MS_time = 10;

        while(Delay_MS_time--);
    }
}





