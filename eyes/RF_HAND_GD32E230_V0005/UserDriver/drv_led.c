/***********************************************************
* @file    drv_power.c
* @brief  电源管理程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_led.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "math.h"




/*
*************************************************************
* 功能说明: 电量指示灯初始化 --修改为 TIMER2 -CH0
* 			PA7--TIMER2-CH0 --pwm输出时，-周期1ms
* 功能说明:  PWM初始化1K
* 形    参: 无
* 返 回 值: 无	 // sys_clk = 72Mhz
*  原先参数计算结果：timer_clk = sys_clk / (prescaler + 1) = 72/720 Mhz = 100k
*                   PWM_frq = timer_clk / (timer_initpara.period +1) = 100/100 k = 1K

*************************************************************
*/
void Bat_Led_Init(void)
{
	/* enable the  GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	gpio_mode_set(LED_BAT_LOW_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_BAT_LOW_PIN);
	gpio_output_options_set(LED_BAT_LOW_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_BAT_LOW_PIN);
	gpio_af_set(LED_BAT_LOW_PORT, GPIO_AF_1, LED_BAT_LOW_PIN);

	gpio_mode_set(LED_BAT_FULL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_BAT_FULL_PIN);
	gpio_output_options_set(LED_BAT_FULL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_BAT_FULL_PIN);
	gpio_af_set(LED_BAT_FULL_PORT, GPIO_AF_1, LED_BAT_FULL_PIN);


	/*********** timer_config **********/
	/* ---------------------------------------------------------------
	TIMER2 Configuration:
	TIMER2CLK = 72M,
	--------------------------------------------------------------- */
	timer_oc_parameter_struct timer_ocinitpara;
	timer_parameter_struct timer_initpara;
	/* enable the TIMER clock */
	rcu_periph_clock_enable(RCU_TIMER2);
	/* deinit a TIMER */
	timer_deinit(TIMER2);
	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&timer_initpara);
	/* TIMER2 configuration 1K */
	timer_initpara.prescaler         	= 719;
	timer_initpara.alignedmode       	= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  	= TIMER_COUNTER_UP;
	timer_initpara.period            	= 99;
	timer_initpara.clockdivision     	= TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter 	= 0;
	timer_init(TIMER2, &timer_initpara);

	/* initialize TIMER channel output parameter struct */
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	/* configure TIMER channel output function */
	timer_ocinitpara.outputstate  	= TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate 	= TIMER_CCXN_DISABLE;
	timer_ocinitpara.ocpolarity   		= TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity  	= TIMER_OCN_POLARITY_HIGH;

	timer_ocinitpara.ocidlestate  	= TIMER_OC_IDLE_STATE_HIGH;  //TIMER_OC_IDLE_STATE_HIGH  TIMER_OC_IDLE_STATE_LOW
	timer_ocinitpara.ocnidlestate 	= TIMER_OCN_IDLE_STATE_HIGH;//TIMER_OCN_IDLE_STATE_HIGH
	timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocinitpara);
	timer_channel_output_config(TIMER2, TIMER_CH_2, &timer_ocinitpara);

	/* configure TIMER channel output pulse value */
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 100);
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 100);
	/* CH0 configuration in OC inactive mode */
	timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	/* configure TIMER channel output shadow function */
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER2);
	/* enable a TIMER */
	timer_enable(TIMER2);
}

/*
*************************************************************
* 功能说明: 低电量指示灯开关
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void Bat_Led_Low_Control(ControlStatus cmd)
{
	if(cmd == ENABLE)
	{
		timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0);

	}
	else
	{
		timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 100);
	}
}

/*
*************************************************************
* 功能说明: 低电量指示灯呼吸控制,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Bat_Led_Low_Breathe(void)
{
	static u8 lev = 0;
	static u8 turn = 1;

	if(turn == 0)
	{
		if(lev >= 1)
		{
			lev --;
		}
		else
		{
			turn = 1;
		}
	}
	else
	{
		lev ++;
		if(lev >= 100)
		{
			turn = 0;
		}
	}
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, lev);
}

/*
*************************************************************
* 功能说明: 高电量指示灯开关
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void Bat_Led_Full_Control(ControlStatus cmd)
{
	if(cmd == ENABLE)
	{
		timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0);
	}
	else
	{
		timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 100);
	}
}

/*
*************************************************************
* 功能说明: 高电量指示灯呼吸控制,需要周期调用
* 形    参: 无
* 返 回 值: 无
				10ms调用一次
*************************************************************
*/
void Bat_Led_Full_Breathe(void)
{
	static u8 lev = 0;
	static u8 turn = 1;

	if(turn == 0)
	{
		if(lev >= 1)
		{
			lev --;
		}
		else
		{
			turn = 1;
		}
	}
	else
	{
		lev ++;
		if(lev >= 100)
		{
			turn = 0;
		}
	}
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, lev);
}





