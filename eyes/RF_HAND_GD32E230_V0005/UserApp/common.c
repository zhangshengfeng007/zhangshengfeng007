#include "common.h"

Debug_data_t Debug_data;

void Data16_Set(u16 *dest,u16 data,u16 len)
{
	u16 i = 0;
	for(i = 0;i < len;i++)
	{
		dest[i] = data;
	}

}

void Data16_Copy(u16 *dest,const u16 *src,u16 len)
{
	u16 i = 0;
	for(i = 0;i < len;i++)
	{
		dest[i] = src[i];
	}
}


