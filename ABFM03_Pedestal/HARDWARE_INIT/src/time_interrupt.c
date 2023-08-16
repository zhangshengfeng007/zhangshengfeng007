
#include "keys.h"

#include "time_interrupt.h"
void MX_TIME_Init(void);
void  Timer8_PWM_Init(void);

__IO uint16_t Delay_Cnt = 0;

void RCC_Configuration(void)
{
    /* PCLK1 = HCLK/4    system clk = 12Mhz*/
    RCC_ConfigPclk1(RCC_HCLK_DIV4);

    /* TIM2 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);
    //When debug ,TIM1 and TIM8 stop
//    DBG_ConfigPeriph(DBG_TIM8_STOP , ENABLE);

}

void  Timer8_PWM_Init(void)
{

		GPIO_InitType GPIO_InitStructure;
		TIM_TimeBaseInitType TIM_TimeBaseStructure;
		OCInitType TIM_OCInitStructure;

		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_TIM8, ENABLE);
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);

		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin            = GPIO_PIN_7;
		GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStructure.GPIO_Current   = GPIO_DC_LOW;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_TIM8;
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.Period    = (1200-1);
		TIM_TimeBaseStructure.Prescaler = (10-1);
		TIM_TimeBaseStructure.ClkDiv    = 0;
		TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
		TIM_InitTimeBase(TIM8, &TIM_TimeBaseStructure);

		TIM_InitOcStruct(&TIM_OCInitStructure);
		TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
		TIM_OCInitStructure.Pulse       = (600);
		TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_LOW;
		TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
		TIM_OCInitStructure.OcIdleState = TIM_OC_IDLE_STATE_SET;
		TIM_InitOc4(TIM8, &TIM_OCInitStructure);

		// ï¿½ï¿½Ê±ï¿½ï¿½Ê¹ï¿½ï¿½
		TIM_EnableCtrlPwmOutputs(TIM8, ENABLE);
		TIM_Enable(TIM8, ENABLE);

}
/********************
 *
 *
 * NVIC
 * *********************/
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/********************
 *
 *
 * ¶¨Ê±Æ÷ÅäÖÃ
 * *********************/
void Time_interrput_Configuration(void)
{
		uint16_t PrescalerValue = 0;
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
	  OCInitType TIM_OCInitStructure;
	  RCC_Configuration();
	  NVIC_Configuration();
	  /* Compute the prescaler value */
    PrescalerValue = (uint16_t)(SystemCoreClock / 12000000) - 1;

	  /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM3, &TIM_TimeBaseStructure);

	  TIM_ConfigPrescaler(TIM3, PrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);


	  TIM_OCInitStructure.OcMode      = TIM_OCMODE_TIMING;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 6000;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
    TIM_InitOc1(TIM3, &TIM_OCInitStructure);

    TIM_ConfigOc1Preload(TIM3, TIM_OC_PRE_LOAD_DISABLE);
	  TIM_ConfigInt(TIM3, TIM_INT_CC1,ENABLE);
	  TIM_Enable(TIM3, ENABLE);
}
void MX_TIME_Init(void)
{
	 RCC_Configuration();
	 Time_interrput_Configuration();
	//  Timer8_PWM_Init();
}


void TIM3_IRQHandler(void)
{
    if (TIM_GetIntStatus(TIM3, TIM_INT_CC1) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_CC1);
		SysInfo.timer_1ms_flag = 1;

		if(Delay_Cnt)
		{
			Delay_Cnt--;
		}

//-------------------
		// sys_key_check();
        // Sys_Time++;


		// RX_Time++;
		// if(Auto_30S_Lock_Cnt<=30000)
		// Auto_30S_Lock_Cnt++;
		LED_Time++;

		// Time_1S++;
		// if(Time_1S>=1000)
		// {
		// 	Time_1S=0;
		// 	if(IDLE_120S_Cnt < 120)
		// 	IDLE_120S_Cnt++;
		// }
		// Lock_Blink_Time++;
		// if(Lock_Blink_Time>=1600)
		// {
		// 	Lock_Blink_Time=0;
		// }

//----------------------
        TIM_SetCmp1(TIM3, (TIM_GetCap1(TIM3) + 6000));  // »Ö¸´¶¨Ê±Æ÷
    }
}

void Delay_MS(uint16_t time)
{
	Delay_Cnt = time;

	while (Delay_Cnt);
}


