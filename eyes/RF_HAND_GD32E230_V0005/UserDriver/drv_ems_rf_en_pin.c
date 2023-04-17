/***********************************************************
* @file    drv_ems.c
* @brief  Ems驱动程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_ems_rf_en_pin.h"
#include "drv_gpio.h"
#include "drv_ems.h"

#include "app_include.h"




static pwm_freq_config_t pwm_200k_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 80,//80//88
};


static pwm_freq_config_t pwm_200k_LEVEL1_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 60,//80//88
};

static pwm_freq_config_t pwm_200k_LEVEL2_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 50,//80//88
};


static pwm_freq_config_t pwm_200k_LEVEL3_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 40,//80//88
};


static pwm_freq_config_t pwm_200k_FULL_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 0,
};

static pwm_freq_config_t pwm_200k_EMETY_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 90,
};


void ems_rf_en_change(en_pin_output_mode_e en_mode)
{
    u8 tmp_pulse = 0;
     switch(en_mode)
     {
        case EN_PIN_EMS_MODE:
        {
            switch(Device.Level)
            {
                case LEVEL1:
                case LEVEL2:
                {
                    tmp_pulse = 60;
                    break;
                }
                case LEVEL3:
                {
                   tmp_pulse = 40;
                    break;
                }
                default:
                {
                   tmp_pulse = 60;
                    break;
                }
            }
            break;
        }

        case EN_PIN_RF_MODE:
        {
            tmp_pulse = 0;
            break;
        }
        case EN_PIN_CLOSE_MODE:
        default:
        {
           tmp_pulse = 90;
            break;
        }
     }
     timer_channel_output_pulse_value_config(TIMER15, TIMER_CH_0, tmp_pulse);
}

/*************************************************************
* PA6---TIMER2 CH0
* 功能说明:  PWM初始化90K
* 形    参: 无
* 返 回 值: 无	 // sys_clk = 72Mhz
*  原先参数计算结果：timer_clk = sys_clk / (prescaler + 1) = 72/4 Mhz = 18MHZ
*                   PWM_frq = timer_clk / (timer_initpara.period +1) = 18/360 mhz = 1/20 MHZ = 50K
*
*
*************************************************************
*/
void timer15_init(void)
{

    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

	pwm_freq_config_t *pwm_config_p = &pwm_200k_EMETY_config;  // 修改，用指针的方式，修改pwm的配置

    /* enable the TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER15);

    /* deinit a TIMER */
    timer_deinit(TIMER15);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = pwm_config_p->prescaler;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = pwm_config_p->period;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER15, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* configure TIMER channel output function */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER15, TIMER_CH_0, &timer_ocinitpara);

    /* configure TIMER channel output pulse value */
    timer_channel_output_pulse_value_config(TIMER15, TIMER_CH_0, pwm_config_p->pulse);
    /* CH0 configuration in OC active mode */
    timer_channel_output_mode_config(TIMER15, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    /* configure TIMER channel output shadow function */
    timer_channel_output_shadow_config(TIMER15, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* TIMER2 primary output function enable */
	timer_primary_output_config(TIMER15, ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER15);

    /* enable a TIMER */
    timer_enable(TIMER15);
}



