/***********************************************************
* @file    drv_rf.c
* @brief  RF驱动程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_rf.h"
#include "math.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "drv_flash.h"
#include "drv_ems_rf_en_pin.h"

RF_Type RF;
extern Debug_data_t Debug_data;

typedef struct {

	u16 Adc_val;
	u16 Vol_val;
	u16 Temp_val;

}TEST_NTC_t;

TEST_NTC_t test_ntc_data;

//20~C --50~C *1000
// const u16 NTC_R_ARRAY[]=
// {
// 	12492, 12213, 11942, 11677, 11419, 11168, 10922, 10683, 10449, 10222,  // 20 --24.5
// 	10000,  9783,  9571,  9365, 91642,  8967,  8776,  8558,  8406,  8228,   // 25 --29.5
// 	8054,   7884,  7718,  7556,  7398,  7244,  7093,  6946,  6802,  6662,   // 30 --34.5
// 	6525,   6391,  6260,  6132,  6007,  5885,  5766,  5650,  5536,  5425,   // 35 --39.5
//     5316,   5210,  5106,  5004,  4905,  4808,  4714,  4621,  4530,  4442,   // 40 --44.5
// 	4355,   4270,  4187,  4106,  4027,  3950,  3874,  3800,  3727,  3656,   // 45 --49.5
// };

//20~C --50~C *1000  : pull_up 10kR,
//
//const u16 NTC_ADC_ARRAY[]=
//{
//	2274, 2252, 2229, 2206, 2183, 2161, 2138, 2115, 2092, 2070,  // 20 --24.5
//	2048, 2025, 2003, 1980, 1955, 1936, 1914, 1888, 1870, 1848,  // 25 --29.5
//	1827, 1805, 1784, 1762, 1741, 1720, 1699, 1678, 1658, 1637,  // 30 --34.5
//	1617, 1597, 1576, 1556, 1537, 1517, 1498, 1478, 1459, 1440,  // 35 --39.5
//	1421, 1403, 1384, 1366, 1347, 1329, 1312, 1294, 1277, 1259,  // 40 --44.5
//	1242, 1225, 1208, 1192, 1175, 1159, 1143, 1127, 1112, 1096,  // 45 --49.5
//};


/*
*************************************************************
* 功能说明: RF初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void RF_Init(void)
{
	RF.State = Func_DISABLE;
	RF.Load = 0;
	RF.Load_First = 0;
	RF.UnLoad_Count = 0;
	RF.Sc_Temp = 250;
	RF.Temp = 250;
	RF.Freq = FREQ_15M;

	rcu_periph_clock_enable(RCU_TIMER0);
	timer_deinit(TIMER0);
	RF_PWM_Config(RF.Freq);
}

/*
*************************************************************
* 功能说明: RF开关控制
* 形    参: cmd 开关
* 返 回 值: 无
*************************************************************
*/
void RF_Control(Func_Type cmd)
{
	if(RF.State == cmd){
		return;
	}
	RF.State = cmd;

	if(RF.State == Func_ENABLE)
	{
		RF_PWM_Config(RF.Freq);
		ems_rf_en_change(EN_PIN_RF_MODE);
	}
	else
	{
		ems_rf_en_change(EN_PIN_CLOSE_MODE);
		RF_PWM_OFF();
	}
}

/*
*************************************************************
* 功能说明: RF频率设置
* 形    参: Freq 频率
* 返 回 值: 无
*************************************************************
*/
void RF_Freq_Set(RF_FREQ_Type Freq)
{
	RF.Freq = Freq;
}

/*
*************************************************************
* 功能说明: RF PWM关闭
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void RF_PWM_OFF(void)
{
	timer_deinit(TIMER0);

	gpio_mode_set(PWM1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWM1_PIN);
	gpio_output_options_set(PWM1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PWM1_PIN);

	PWM1_OFF;
}

/*
*************************************************************
* 功能说明: RF PWM初始化,1M 0%
* 形    参: Freq 需要输出的频率
* 返 回 值: 无
*************************************************************
*/
void RF_PWM_Config(RF_FREQ_Type Freq)
{
	timer_oc_parameter_struct timer_ocinitpara;
	timer_parameter_struct timer_initpara;

	/*configure  as alternate function*/
	gpio_mode_set(PWM1_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM1_PIN);
	gpio_output_options_set(PWM1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM1_PIN);
	gpio_af_set(PWM1_PORT, GPIO_AF_2, PWM1_PIN);

	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&timer_initpara);
	/* TIMER0 configuration */
	timer_initpara.prescaler 	= 0;
	timer_initpara.alignedmode 	= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	if(RF.Freq == FREQ_1M){
		timer_initpara.period = 71;
	}else if(RF.Freq == FREQ_125M){
		timer_initpara.period = 57;
	}else if(RF.Freq == FREQ_15M){
		timer_initpara.period = 47;
	}

	timer_initpara.clockdivision 	= TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER0, &timer_initpara);

	/* initialize TIMER channel output parameter struct */
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	/* CH1/CH1N configuration in PWM mode0 */
	timer_ocinitpara.outputstate		= TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate	= TIMER_CCXN_ENABLE;
	timer_ocinitpara.ocpolarity		= TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity		= TIMER_OCN_POLARITY_HIGH;
	timer_ocinitpara.ocidlestate		= TIMER_OC_IDLE_STATE_LOW;
	timer_ocinitpara.ocnidlestate		= TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocinitpara);

	/* configure TIMER channel 0 output pulse value */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, (timer_initpara.period+1)/2);
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


/*
*************************************************************
* 功能说明: RF 输出频率控制程序，需要周期性调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void RF_OutFreq_Cycle(void)
{
	//按温度设置频率
	if(RF.Temp  < 370){
		RF_Freq_Set(FREQ_15M);
	}else if(RF.Temp  < 390){
		RF_Freq_Set(FREQ_125M);
	}else{
		RF_Freq_Set(FREQ_1M);
	}
}

/*
*************************************************************
* 功能说明: RF 温度读取,需要周期调用
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void RF_Temp_Read(void)
{
	static u16 arr[10] = {0};
	static u8 count = 0;
	u16 data = 0;
	u8 i = 0;
	u8 j = 0;

	u8 cur_index, p_tail, p_head, last_index = 0;
	u16 tmp;

	if(count < 10)
	{
		arr[count] = adc_value[0];
		count++;
		return;
	}

	count = 0;

	//冒泡排序取中值
	for(i = 0; i < 10-1; i++){
		for(j = 0; j < 10-i-1; j++){
			if(arr[j] > arr[j+1]){
				data = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = data;
			}
		}
	}


	if(arr[4] < 3800)
	{

//		// 二分法查表
//		p_tail = sizeof(NTC_ADC_ARRAY) / sizeof(NTC_ADC_ARRAY[0]);
//		cur_index = 0;
//		last_index = p_tail;
//		p_head = 0;
//		for (i = 0; i < sizeof(NTC_ADC_ARRAY) / sizeof(NTC_ADC_ARRAY[0]); i++)
//		{
//			if((arr[4] >= NTC_ADC_ARRAY[cur_index]))
//			{
//				if((0 == cur_index)||(1 == cur_index))
//				{
//					break;
//				}

//				if(arr[4] <= NTC_ADC_ARRAY[cur_index - 1])
//				{
//					cur_index = cur_index - 1;
//					break;
//				}
//				tmp = cur_index;
//				cur_index = (cur_index + p_head) / 2;
//				last_index = tmp;
//			}
//			else
//			{
//				if((p_tail == cur_index) || (p_tail -1 == cur_index))
//				{
//					break;
//				}
//				if(arr[4] >= NTC_ADC_ARRAY[cur_index + 1])
//				{
//					cur_index = cur_index + 1;
//					break;
//				}
//				tmp = cur_index;
//				cur_index = (cur_index + last_index) / 2;
//				p_head = tmp;    //  last_index
//			}
//		}
		// //计算电阻
		 arr[4] =  arr[4]*10000/(4095-arr[4]);

		//计算温度,1位小数点
//		 RF.Sc_Temp = ((1/( log(arr[4]/10000.0)/3950.0 + 0.003354)) - 273.15)*10;
		RF.Sc_Temp = ((1/( log(arr[4]/10000.0)/3380.0 + 0.003354)) - 273.15)*10;
//		RF.Sc_Temp = 200 + cur_index * 5;
		RF.Temp = RF.Sc_Temp + RF.Ntc_Offset;
	}
	else
	{
		RF.Sc_Temp = 800;
		RF.Temp = 800;
	}
}

/*
*************************************************************
* 功能说明: RF 负载检测,需要周期调用
* 形    参: 无
* 返 回 值: 无
*           1ms 调用一次
*  ADC值  3.3v ---4096
*  测试到的数据 有负载时， 输出约为 2.4v    ---//用2.2v计算 约为2703  ---后期需再调整
                无负载时，输出约为 2.08v   ---// 约为      2581
*************************************************************
*/
void RF_Load_Detection(void)
{


	static u8 active_cnt = 0;
	static u8 unactive_cnt = 0;

	if(RF.State == Func_DISABLE)
	{
		return;
	}


	// count1 ++;
	//if(count1 < _OUT_TIME_)
	//{
	// if(adc_value[3] > 1100)  // 2703
	if(adc_value[3] >= 1100)
	{
		// count2 ++;
		active_cnt ++;
		if(active_cnt > 10)
		{
			RF.Load = 1;
			active_cnt = 11;
		}
		unactive_cnt = 0;
		Debug_data.dbg_val = adc_value[3];
	}
	//}
	else
	{
		unactive_cnt ++;
		Debug_data.dbg_val = adc_value[3];
		if(unactive_cnt >= 10)
		{
			unactive_cnt = 11;
			RF.Load = 0;
		}
		active_cnt = 0;
	}

}


