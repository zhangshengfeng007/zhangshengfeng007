/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2019-02-19, V1.0.0, firmware for GD32E23x
    \version 2020-12-12, V1.1.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32e23x.h"
#include <stdio.h>
#include "main.h"
#include "common.h"

#include "app_include.h"


static void switch_system_clock_to_72m_irc8m(void);
static void systick_config(void);
static void irc40k_config(void);


#define APP_OFFSET	0x2800		// 10K
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
	
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, APP_OFFSET);
	/* enable global interrupt, the same as __set_PRIMASK(0) */
	 __enable_irq();
	
	u8 Sys_Index = 0;

	/* switch system clock to 72MHz by IRC8M */
	switch_system_clock_to_72m_irc8m();

#if  _WDT_ENABLE
	/* enable write access to FWDGT_PSC and FWDGT_RLD registers.
       FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
       irc40k_config();
	fwdgt_config(625, FWDGT_PSC_DIV64);	//配置为1S后复位
	fwdgt_enable();
#endif

	/******* 驱动 ********/
	systick_config();	//系統滴答定时器1MS，任务使用
	Delay_Init();
	GPIO_Init();

	Adc_Init();
	Key_Hardware_Init();
	Motor_Init();
	Power_Init();
	Sensor_Init();
	RF_Init();
	Ems_Init();
	Drv_Uart_Init();

	timer15_init();
	/******* 应用 ********/
	Key_App_Init();
	Device_Init();
	Bat_Led_Init();

//	Test_Init();

	while(1){
		/*************** TASK RUN   *****************/
		for(Sys_Index = 0; Sys_Index < TASK_MAX; Sys_Index++)
		{
			if(Task_Time_Arr[Sys_Index] == 0)
			{
				Task_Arr[Sys_Index]();
			}
		}
	}
}

/*!
    \brief      switch system clock to 72M by IRC8M
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void switch_system_clock_to_72m_irc8m(void)
{
	uint32_t timeout = 0U;
	uint32_t stab_flag = 0U;

	/* select IRC8M as system clock source, deinitialize the RCU */
	rcu_system_clock_source_config(RCU_CKSYSSRC_IRC8M);
	rcu_deinit();

	/* enable IRC8M */
	RCU_CTL0 |= RCU_CTL0_IRC8MEN;

	/* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
	do{
		timeout++;
		stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
	}
	while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

	/* if fail */
	if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)){
		while(1){
		}
	}

	FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_2;

	/* AHB = SYSCLK */
	RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
	/* APB2 = AHB */
	RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
	/* APB1 = AHB */
	RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
	/* PLL = (IRC8M/2) * 18 = 72 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
	RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL18);

	/* enable PLL */
	RCU_CTL0 |= RCU_CTL0_PLLEN;

	/* wait until PLL is stable */
	while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)){
	}

	/* select PLL as system clock */
	RCU_CFG0 &= ~RCU_CFG0_SCS;
	RCU_CFG0 |= RCU_CKSYSSRC_PLL;

	/* wait until PLL is selected as system clock */
	while(0U == (RCU_CFG0 & RCU_SCSS_PLL)){
	}
}

/*!
    \brief      IRC40K configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void irc40k_config(void)
{
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait till IRC40K is ready */
    rcu_osci_stab_wait(RCU_IRC40K);
}

/*!
    \brief      configure systick
    \param[in]  设置滴答定时器1MS中断
    \param[out] none
    \retval     none
*/
static void systick_config(void)
{
	/* systick clock source is from HCLK/8 */
	systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);
	SysTick_Config(SystemCoreClock / 1000U);
	/* configure the systick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x00U);
}



