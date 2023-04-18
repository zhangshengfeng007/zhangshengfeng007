/***********************************************************
* @file    drv_flash.c
* @brief  flash��������
* @version V1.0
* @date    2022.12.05
***********************************************************/
#include "drv_flash.h"

/*
*************************************************************
* ����˵��: flash����
* ��    ��: addr��Ҫ�����ĵ�ַ
* �� �� ֵ: ��
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
* ����˵��: ����д���ݵ�flash   ----�����汾
* ��    ��: addrд���ַ��data����ָ�Len���ݳ���
* �� �� ֵ: ��

FMC�ṩ��һ��DBUS 32λ����/16λ���ֱ�̹��ܣ������޸�������洢�������ݡ�Ȼ��ʵ
���ϣ� ����FMC_WS�Ĵ�����PGWλ������32λ����64λ���ݱ���̵�����洢�������²�
����ʾ�˱�̲����Ĵ����Ĺ��̣�
1. ȷ��FMC_CTL�Ĵ�������������״̬��
2. ���FMC_STAT�Ĵ�����BUSYλ��ȷ������洢��û�����ڽ����еĲ����� ��BUSYλ
   Ϊ0�� ����ȴ��ò�����ɣ�
3. �������Ҫ������PGWλ��
4. д�ֱ�����FMC_CTL�Ĵ�����PGλ��
5. DBUSдһ�����ݵ�Ԥ�ھ��Ե�ַ��0x08XX XXXX����

a. ���DBUSд����һ��32λ���ֲ���PGWλ�����ó�0��32λ�������洢������ DBUSдһ
   �����ݱ���̵�����洢���� �ڱ������ʱ�������ֶ��롣
b. ���DBUSд����һ��32λ���ֲ���PGWλ�����ó�1��64λ�������洢������ DBUSд��
   �����һ��64λ���ݣ� Ȼ���ٱ�̵�����洢���� �ڱ������ʱ������˫�ֶ��롣
c. ���DBUSд����һ��16λ���ֲ���PGWλ�����ó�0��32λ�������洢������ DBUSд��
   �����һ��32λ���ݣ� Ȼ���ٱ�̵�����洢���� �ڱ������ʱ�������ֶ��롣
d. ���DBUSд����һ��16λ���ֲ���PGWλ�����ó�1��64λ�������洢������ DBUSд��
   �����һ��64λ���ݣ� Ȼ���ٱ�̵�����洢���� �ڱ������ʱ������˫�ֶ��롣

Ϊ�˼��ٱ��ʱ�䣬�������DBUS 32λ��̣� �������̵�������˫�ֶ�����PGWλ��
1�� �������̵��������ֶ�����PGWλ���㡣
 a).ͨ�����FMC_STAT�Ĵ�����BUSYλ���жϲ���ָ���Ƿ�ִ����ϣ� ��δ�������ȴ�
    BUSYλΪ0��
 b).�������Ҫ������ʹ��DBUS������֤������洢���Ƿ��̳ɹ���


�����洢���̳ɹ�ִ�У� FMC_STAT �Ĵ����� ENDF λ���ᱻ�� 1��
  ������� FMC_CTL�Ĵ����� ENDIE λ֮ǰ�Ѿ����� 1�� ��ô FMC ������һ���жϡ�

��Ҫע�����:************************
��ִ����/���ֱ�̲���֮ǰ��Ҫ���Ŀ�ĵ�ַ�Ƿ��Ѿ�����������
 ���û�б��������� PGERR λ������1����ҳ�ϵı�̲�����Ч��ʹ�������Ϊ 0x0��
    ÿ�����ڲ�������´β���ǰֻ�ܱ��һ�Ρ�
��һ���棬 �ڲ���/��̱�����ҳ���б�̲���������Ч�� ��� FMC_CTL �Ĵ����� ERRIE λ
���� 1�� FMC ������һ��������������жϡ�������Լ�� FMC_STAT �Ĵ����� PGERR λ
������жϴ�������״���� FMC_STAT �Ĵ����� ENDF λָʾ�����Ľ�����

     ----statrt----
            |
		Is the LK bit 0  ---(NO)--�� Unclock the FMC_CTL
		    |
			|Yes
        Is the Busy bit 0 --(NO)--��Back check��
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





