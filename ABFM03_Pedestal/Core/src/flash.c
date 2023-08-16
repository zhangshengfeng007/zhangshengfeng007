#include "flash.h"


#define  USERDATA_STRAT_ADDR	  0x0800F800
#define  USERDATA_SIZE	                  2048

#define PEERCODE_START_ADDR	0x0800F800
#define PEERCODE_SIZE		512




uint32_t ReadPeerCode(void)
{
    uint32_t PeerCode= (*(__IO uint32_t*)(PEERCODE_START_ADDR));

	return PeerCode;
}


void SavePeerCode(uint32_t PeerCode)
{
    __disable_irq();	
    FLASH_Unlock();
    if (FLASH_COMPL != FLASH_EraseOnePage(PEERCODE_START_ADDR))
    {
        while(1)
        {
            printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
        }
    }

    if (FLASH_COMPL != FLASH_ProgramWord(PEERCODE_START_ADDR, PeerCode))
    {
        while(1)
        {
            printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
        }
    }
    FLASH_Lock();
   __enable_irq();
}

