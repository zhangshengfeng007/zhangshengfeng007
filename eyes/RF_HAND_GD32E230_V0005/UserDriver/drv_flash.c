/***********************************************************
* @file    drv_flash.c
* @brief  flash驱动程序
* @version V1.0
* @date    2022.12.05
***********************************************************/
#include "drv_flash.h"

/*
*************************************************************
* 功能说明: flash擦除
* 形    参: addr需要擦除的地址
* 返 回 值: 无
*************************************************************
*/
void Flash_Erase_Page(uint32_t addr)
{
	uint32_t num = 0;

	num = (addr - FMC_START_ADDR)/FMC_PAGE_SIZE;

	/* unlock the flash program/erase controller */
	fmc_unlock();

	/* clear all pending flags */
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	fmc_page_erase(FMC_START_ADDR + (FMC_PAGE_SIZE * num));
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

	/* lock the main FMC after the erase operation */
	fmc_lock();
}

/*
*************************************************************
* 功能说明: 按字写数据到flash   ----佳明版本
* 形    参: addr写入地址，data数据指令，Len数据长度
* 返 回 值: 无

FMC提供了一个DBUS 32位整字/16位半字编程功能，用来修改主闪存存储器的内容。然而实
际上， 是由FMC_WS寄存器中PGW位来设置32位或者64位数据被编程到闪存存储器。以下步
骤显示了编程操作寄存器的过程：
1. 确保FMC_CTL寄存器不处于锁定状态；
2. 检查FMC_STAT寄存器的BUSY位来确保闪存存储器没有正在进行中的操作， 即BUSY位
   为0。 否则等待该操作完成；
3. 如果有需要，设置PGW位；
4. 写字编程命令到FMC_CTL寄存器的PG位；
5. DBUS写一个数据到预期绝对地址（0x08XX XXXX）；

a. 如果DBUS写的是一个32位整字并且PGW位被设置成0（32位编程闪存存储器）， DBUS写一
   次数据被编程到闪存存储器。 在编程数据时必须是字对齐。
b. 如果DBUS写的是一个32位整字并且PGW位被设置成1（64位编程闪存存储器）， DBUS写两
   次组成一个64位数据， 然后再编程到闪存存储器。 在编程数据时必须是双字对齐。
c. 如果DBUS写的是一个16位半字并且PGW位被设置成0（32位编程闪存存储器）， DBUS写两
   次组成一个32位数据， 然后再编程到闪存存储器。 在编程数据时必须是字对齐。
d. 如果DBUS写的是一个16位半字并且PGW位被设置成1（64位编程闪存存储器）， DBUS写四
   次组成一个64位数据， 然后再编程到闪存存储器。 在编程数据时必须是双字对齐。

为了减少编程时间，假设采用DBUS 32位编程， 如果被编程的数据是双字对齐则将PGW位置
1， 如果被编程的数据是字对齐则将PGW位清零。
 a).通过检查FMC_STAT寄存器的BUSY位来判断擦除指令是否执行完毕， 若未完成则需等待
    BUSY位为0；
 b).如果有需要，可以使用DBUS读并验证该闪存存储器是否编程成功。


当主存储块编程成功执行， FMC_STAT 寄存器的 ENDF 位将会被置 1，
  并且如果 FMC_CTL寄存器的 ENDIE 位之前已经被置 1， 那么 FMC 将触发一个中断。

需要注意的是:************************
在执行字/半字编程操作之前需要检查目的地址是否已经被擦除过，
 如果没有被擦除过， PGERR 位将被置1，该页上的编程操作无效即使编程内容为 0x0。
    每个字在擦除后和下次擦除前只能编程一次。
另一方面， 在擦除/编程保护的页进行编程操作将会无效。 如果 FMC_CTL 寄存器的 ERRIE 位
被置 1， FMC 将触发一次闪存操作错误中断。软件可以检查 FMC_STAT 寄存器的 PGERR 位
来监测中断处理器的状况。 FMC_STAT 寄存器的 ENDF 位指示操作的结束。

     ----statrt----
            |
		Is the LK bit 0  ---(NO)--》 Unclock the FMC_CTL
		    |
			|Yes
        Is the Busy bit 0 --(NO)--（Back check）
		    |
			|YES
		Set the PG bit
			|
		Perform world/half word write by DBUS
		    |
		Is the Busy bit 0 ---(NO)--(Back check)
			|
			|YES
	 ---Finish-------

*************************************************************
*/
void Flash_Write_Word(uint32_t addr,uint32_t *data,u16 Len)
{
	uint32_t num = 0;
	u16 i = 0;

	 num = (addr - FMC_START_ADDR)/FMC_PAGE_SIZE;

	/* unlock the flash program/erase controller */
	fmc_unlock();

	 /* clear all pending flags */
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	fmc_page_erase(FMC_START_ADDR + (FMC_PAGE_SIZE * num));
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

	for(i = 0; i < Len; i++)
	{
		fmc_word_program(addr+(i*4), data[i]);
	}
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

	// /* lock the main FMC after the erase operation */
	 fmc_lock();
}





