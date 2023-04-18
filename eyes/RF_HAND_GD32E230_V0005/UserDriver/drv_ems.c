/***********************************************************
* @file    drv_ems.c
* @brief  Ems��������
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_ems.h"
#include "drv_gpio.h"
#include "drv_ems_rf_en_pin.h"
#include "drv_rf.h"
#include "drv_adc.h"
#include "drv_power.h"

extern RF_Type RF;
Ems_Type Ems;

/*
*************************************************************
* ����˵��: EMS��ʼ��
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Ems_Init(void)
{
	//GPIO��TIMER�Ѿ���RF�����г�ʼ��
	Ems.State = Func_DISABLE;
}

/*
*************************************************************
* ����˵��: Ems���ؿ���
* ��    ��: cmd ����
* �� �� ֵ: ��
*************************************************************
*/
void Ems_Control(Func_Type cmd)
{
	if(Ems.State == cmd){
		return;
	}
	Ems.State = cmd;

	if(Ems.State == Func_ENABLE)
	{
		Ems_PWM_Config();
		//ems_rf_en_change(EN_PIN_EMS_MODE);
		ems_rf_en_change(EN_PIN_RF_MODE);
	}
	else
	{
		ems_rf_en_change(EN_PIN_CLOSE_MODE);
		Ems_PWM_OFF();
	}
}

/*
*************************************************************
* ����˵��: Ems PWM�ر�ȫ��Ϊ�͵�ƽ
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Ems_PWM_OFF(void)
{
	timer_deinit(TIMER0);

	gpio_mode_set(PWM1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWM1_PIN);
	gpio_output_options_set(PWM1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PWM1_PIN);

	PWM1_OFF;
}



static pwm_freq_config_t pwm_5k_config ={
	.prescaler = 8,
	.period = 1599,
	.pulse = 800,
};


static pwm_freq_config_t pwm_50k_config ={
	.prescaler = 3,
	.period = 359,
	.pulse = 180,
};

static pwm_freq_config_t pwm_90k_config ={
	.prescaler = 3,
	.period = 199,
	.pulse = 100,
};



//pwm_freq_config_t pwm_100k_config ={
//	.prescaler = 3,
//	.period = 179,
//	.pulse = 90,
//};


static pwm_freq_config_t pwm_100k_config ={
	.prescaler = 3,
	.period = 179,
	.pulse = 90,
};



static pwm_freq_config_t pwm_200k_config ={
	.prescaler = 3,
	.period = 89,
	.pulse = 45,
};


static pwm_freq_config_t pwm_250k_config ={
	.prescaler = 3,
	.period = 71,
	.pulse = 36,
};    // 250k


static pwm_freq_config_t pwm_300k_config ={
	.prescaler = 3,
	.period = 59,
	.pulse = 30,
};    // 300k


static pwm_freq_config_t pwm_500k_config ={
	.prescaler = 3,
	.period = 35,
	.pulse = 18,
};    // 500k





/*
*************************************************************
* ����˵��: Ems PWM��ʼ��90K
* ��    ��: ��
* �� �� ֵ: ��	 // sys_clk = 72Mhz
*  ԭ�Ȳ�����������timer_clk = sys_clk / (prescaler + 1) = 72/4 Mhz = 18MHZ
*                   PWM_frq = timer_clk / (timer_initpara.period +1) = 18/360 mhz = 1/20 MHZ = 50K
*
*
*************************************************************
*/
void Ems_PWM_Config(void)
{

	pwm_freq_config_t *pwm_config_p = &pwm_100k_config;  // �޸ģ���ָ��ķ�ʽ���޸�pwm������

	timer_oc_parameter_struct timer_ocinitpara;
	timer_parameter_struct timer_initpara;

	/*configure  as alternate function*/
	gpio_mode_set(PWM1_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM1_PIN);
	gpio_output_options_set(PWM1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM1_PIN);
	gpio_af_set(PWM1_PORT, GPIO_AF_2, PWM1_PIN);

	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&timer_initpara);
	/* TIMER0 configuration */
	timer_initpara.prescaler = pwm_config_p->prescaler;          // timer_clk = sys_clk/(prescaler + 1)

	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = pwm_config_p ->period;        // ʵ�� �������� = ��period + 1��
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER0, &timer_initpara);

	/* initialize TIMER channel output parameter struct */
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	/* CH1/CH1N configuration in PWM mode0 */
	timer_ocinitpara.outputstate		= TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate	= TIMER_CCXN_DISABLE;
	timer_ocinitpara.ocpolarity		= TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity		= TIMER_OCN_POLARITY_HIGH;
	timer_ocinitpara.ocidlestate		= TIMER_OC_IDLE_STATE_LOW;
	timer_ocinitpara.ocnidlestate		= TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocinitpara);

	/* configure TIMER channel 0 output pulse value */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, pwm_config_p->pulse);    // ռ�ձ�50% ��pulse_param = (timer_initpara.period +1�� /2
	/* configure TIMER channel 0 output compare mode */
	timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	/* disable TIMER channel 0 output shadow function */
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

	/* TIMER0 primary output function enable */
	timer_primary_output_config(TIMER0, ENABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER0);

	/* TIMER0 counter enable */
	timer_enable(TIMER0);
}


/*******************************************************************************
 * 	��ΪӲ���Ĳ��죬����rf���ʱ���޷�ʶ�����Ը�ΪEMS�첨�����ems���ʱ�ĵ�ѹ
 *
 **  ���Ե������� �и���ʱ��    ���ԼΪ 2.3v    ---//��2.0v���� ԼΪ2482  ---�������ٵ���
                �޸���EMSʱ�ˣ����ԼΪ 1.6v   ---// ԼΪ    1.6*4096/3.3 =   1985
 *
 *			����300ms��ʼ��⣬

��ΪӲ������ѹ��·��ԭ�򣺵���ص�ѹ�µͣ�����������ĵ�����Խ�������غ� Vin *Iin = Vout *Iout��
*
*  ��ص�ѹ             ����ems���ʱT9��ѹ          ����ems���ʱT9��ѹ        ���ؼ���ѹ
*   4.2v                 1.6v                        2.3v                    2.1v
*   4.1v                 1.73v
*   4.0v                 1.77v
*   3.9v                 1.81v
*   3.8v                 1.89v
*   3.7v                 1.96v                       2.64                    2.1v
------------------------------------------------------------------------------------
*   3.6v                 2.0v                        2.64v                   2.5v
*   3.5v                 2.0v                        2.64v                   2.5v
*   3.4v                 2.1v                        3.3v                    2.5v
*   3.3v                 2.3v                        3.3v                    2.5v
*   3.2v                 2.3v                        3.3v                    2.5v
************************************************************************************/
void EMS_Load_Detection(void)
{
	static u8 active_cnt = 0;
	static u8 unactive_cnt = 0;
	static u16 check_active_delay = 0;
	// u16 RF_Load_adc_val = T9_2V1_ADC_VAL;
	u16 RF_Load_adc_val = RF.UnLoad_ADC_Val + T9_0V5_ADC_VAL;

	check_active_delay ++;
	if(check_active_delay < 1500)
	{
		return;
	}
	check_active_delay = 1500;


	if(Ems.State == Func_DISABLE)
	{
		return;
	}

	Debug_data.dbg_val = adc_value[3];
	RF.POWER_DETECT_ADC_Val = adc_value[3];
	if(Power.adc_val < BAT_3V7_ADC_VAL)
	{
		// RF_Load_adc_val = T9_2V5_ADC_VAL;
		RF_Load_adc_val = RF.UnLoad_ADC_Val + T9_0V5_ADC_VAL * 2;
	}

	if(adc_value[3] >= RF_Load_adc_val)
	{
		active_cnt ++;
		if(active_cnt > 5)
		{
			if(0 ==	RF.Load)
			{
				// printf("check load active\r\n");
			}
			RF.Load = 1;
			active_cnt = 11;
		}
		unactive_cnt = 0;
	}
	else
	{
		unactive_cnt ++;
		if(unactive_cnt >= 10)
		{
			unactive_cnt = 11;
			RF.Load = 0;
		}
		active_cnt = 0;
	}
}



