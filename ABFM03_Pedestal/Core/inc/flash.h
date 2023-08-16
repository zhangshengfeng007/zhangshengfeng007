#ifndef __FLASH_H__
#define __FLASH_H__

#include "User_System_Config.h"

uint32_t ReadPeerCode(void);
void SavePeerCode(uint32_t PeerCode);
#endif /*__ FLASH_H__ */
