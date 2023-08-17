#include "includes.h"


uint16_t ADC_Value[ADC_ADD_COUNT][ADC_CHANNEL_NUM]={0};
uint16_t Bat_ADC_Value[BAT_ADC_COUNT]={0};
BAT_DATA_t BAT_Data;
/**************************************************************************************
* FunctionName   : USB_Plug_Scan(void)
* Description    :
* EntryParameter :
* ReturnValue    : None  123
**************************************************************************************/
void USB_Plug_Scan(void)	//10ms����һ��
{
	static uint8_t usb_plug_time;
	static uint8_t usb_no_plug_time;

	//if(VIN_DET_IN() && BAT_CHARGE_STATE_IN())
	if(DC_5V_INEXIST())
	{
		usb_no_plug_time ++;
		// BOOST_5V_ON();
		if(usb_no_plug_time >= 5)
		{
			if(SysInfo.Batt_Value.Usb_flag)
			{
				//SysInfo.Save_Data.save_Data_flag =1;	//2023 07 28 拔掉充电器后，电压出现短暂的不稳定（150ms左右），并且电池没有被拔掉的情况下，SysInfo.Batt_Value.State不会改变
				SysInfo.Save_Data.BattState	 =SysInfo.Batt_Value.State;
				//BOOST_5V_ON();
			}
			SysInfo.Batt_Value.Usb_flag = 0;
			usb_no_plug_time = 5;
		}
		usb_plug_time = 0;
	}
	else
	{
		usb_plug_time ++;

		if(usb_plug_time >= 5)
		{
			SysInfo.Batt_Value.Usb_flag = 1;
			usb_plug_time = 5;
			BOOST_5V_OFF();  // 停止输出，电源管理IC 进入充电模式
		}
		usb_no_plug_time = 0;
	}
}

/*************************************************************************************
* FunctionName	 : filter()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
uint16_t filter(uint16_t *pData, uint8_t channel, uint8_t len, uint8_t count)
{
  uint16_t value_buf1[ADC_ADD_COUNT] = {0};
  uint8_t i, j, k;
  uint16_t temp;

  pData = pData + channel;
  for(k = 0; k < count; k ++)
  {
    value_buf1[k] =(uint16_t) *pData;
    pData = pData + len;
  }

  for(j = 0; j < count - 1; j ++)
  {
    for (i = 0; i < count - j - 1; i ++)
    {
      if (value_buf1[i] > value_buf1[i+1])
      {
        temp = value_buf1[i];
        value_buf1[i] = value_buf1[i+1];
        value_buf1[i+1] = temp;
      }
    }
  }
  return value_buf1[(count-1)/2];
}

/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void ADC_DMA_COV_START(void)
{
	//BAT_OUT_ENABLE();
	BOOST_3V3_ON();
	//HAL_Delay(10);// 增加延时后，在开始采样阶段会有一段时间的 浮高数据
  HAL_ADCEx_Calibration_Start(&hadc1);
  //HAL_Delay(10);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADC_Value, ADC_CHANNEL_NUM*ADC_ADD_COUNT);
//	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,ADC_CHANNLE_NUM);
}

/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
#if 0
  DMA_Transfer_Complete_Count++;
#endif
  //CHECK_BATT_ADCIN_DIS;
//  HAL_ADC_Stop_DMA(&hadc1);
//	SysInfo.ADC_Value.ADC_VEMS_Feedback_VALUE  	= filter((uint16_t *)&ADC_Value,0,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
////	SysInfo.ADC_Value.ADC_TEMP_VALUE       			= filter((uint16_t *)&ADC_Value,1,ADC_CHANNLE_NUM,ADC_ADD_COUNT);
////  SysInfo.ADC_Value.ADC_BAT_VALUE       			= filter((uint16_t *)&ADC_Value,2,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
//  SysInfo.ADC_Value.ADC_RF_VALUE        			= filter((uint16_t *)&ADC_Value,3,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
//  SysInfo.ADC_Value.ADC_EMS_DET_VALUE        	= filter((uint16_t *)&ADC_Value,4,ADC_CHANNEL_NUM,ADC_ADD_COUNT);
//  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,ADC_CHANNEL_NUM*ADC_ADD_COUNT);
}
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
uint16_t Get_Battery_Read(uint16_t* ADC_Source,uint16_t Counts)
{
	uint8_t i;
	uint32_t max, min, sum, ad_temp;

	max = 0x00;
	min = 0x0FFF;
	sum = 0x00;

	/* ��ͨ��������10��ת����������ͨ������ */
	for (i=0; i<Counts; i++)
	{
		ad_temp = ADC_Source[i];
		if (ad_temp > max)
		{
			max = ad_temp;
		}
		if (ad_temp < min)
		{
			min = ad_temp;
		}
		sum += ad_temp;
	}
	sum -= min;
	sum -= max;

	return sum>>7;  //ȡ128��ƽ��ֵ
}
/**************************************************************************************
* FunctionName   : void Charge_Batter_State()
* Description    : 130ms����һ��
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void CheckCharge_Batter_State(void)
{
	if(BAT_Data.adc_Value <= BAT_VOL_3V55  &&  (BAT_Data.adc_Value > BAT_VOL_3V05))			//0   2499
	{
		BAT_Data.status = BAT_00_00_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V80 && (BAT_Data.adc_Value > BAT_VOL_3V55))	//0~20   2499 ~ 3154
	{
		BAT_Data.status = BAT_00_20_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V85 && (BAT_Data.adc_Value > BAT_VOL_3V80))	//20~40  3154 ~ 3277  BAT_VOL_3V85  BAT_VOL_3V95  BAT_VOL_4V05
	{
		BAT_Data.status = BAT_20_30_STATUS;
		// TimeCount = 0;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V95 && (BAT_Data.adc_Value > BAT_VOL_3V85))	//20~40  3154 ~ 3277
	{
		BAT_Data.status = BAT_30_40_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_4V05 && (BAT_Data.adc_Value > BAT_VOL_3V95))	//40~60  3277 ~ 3317
	{
		BAT_Data.status = BAT_40_60_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_4V10 && (BAT_Data.adc_Value > BAT_VOL_4V05))   //60~80  3317 ~ 3359
	{
		BAT_Data.status = BAT_60_70_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_4V17 && (BAT_Data.adc_Value > BAT_VOL_4V10))   //60~80  3317 ~ 3359
	{
		BAT_Data.status = BAT_70_80_STATUS;
	}
	else if(BAT_Data.adc_Value >BAT_VOL_4V17 ) //80~100   3359
	{
		BAT_Data.status = BAT_80_100_STATUS;
	}

	if(BAT_CHARGE_FULL())
	{

		BAT_Data.charge_ic_pin_full_disp_delay ++;
		if(BAT_Data.charge_ic_pin_full_disp_delay >= 200)//26s 200*130MS
		{
			BAT_Data.charge_ic_pin_full_disp_delay = 200 ;
			BAT_Data.status = BAT_100_100_STATUS;
		}
	}
	else if(BAT_Data.status == BAT_80_100_STATUS)
	{

		BAT_Data.bat_lvl_full_disp_delay ++;  // 130ms * 1000 = 130s
		if (BAT_Data.bat_lvl_full_disp_delay >  20000)  //130ms * 20000 = 2600s / 60 = 43min
		{
			BAT_Data.bat_lvl_full_disp_delay =  20000;
			BAT_Data.status = BAT_100_100_STATUS;  //
		}
	}

}
/**************************************************************************************
* FunctionName   : uint8_t CheckIdle_Batter_State(uint16_t CheckBatValue)
* Description    : 130ms
* EntryParameter :
* 根据放电曲线： 0.2c放电时，即空载情况。（查询电量）
*  将放电时间平均分为 3份：
*   从 4.2v ~ 3.85v  ----  3格电量等级  BAT_40_60_STATUS
*   从 3.85v ~ 3.65v ----  2格电       BAT_20_30_STATUS,BAT_30_40_STATUS
*   从 3.55v ~ 3.65v ----  1格电       BAT_00_20_STATUS
*   从 3.55v 以下低电提示
* ReturnValue    : None
**************************************************************************************/
void CheckIdle_Batter_State(void)
{
	// static uint8_t IdleBattState;

	if(BAT_Data.adc_Value <= BAT_VOL_3V55 && (BAT_Data.adc_Value > BAT_VOL_3V05))		  //0       2908
	{
		BAT_Data.status = BAT_00_00_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V65 && (BAT_Data.adc_Value > BAT_VOL_3V55))	 //0~20     2908  ~  3072
	{
		BAT_Data.status = BAT_00_20_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V80 && (BAT_Data.adc_Value > BAT_VOL_3V65))	 //20~40    3072  ~  3113
	{
		BAT_Data.status = BAT_20_30_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V85 && (BAT_Data.adc_Value > BAT_VOL_3V80))	 //20~40    3113  ~  3154
	{
		BAT_Data.status = BAT_30_40_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_3V95 && (BAT_Data.adc_Value > BAT_VOL_3V85))	 //40~60    3154  ~  3236
	{
		BAT_Data.status = BAT_40_60_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_4V05 && (BAT_Data.adc_Value > BAT_VOL_3V95))  //60~70      3236  ~  3317
	{
		BAT_Data.status = BAT_60_70_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_4V10 && (BAT_Data.adc_Value > BAT_VOL_4V05))  //70~80      3317  ~  3359
	{
		BAT_Data.status = BAT_70_80_STATUS;
	}
	else if(BAT_Data.adc_Value <= BAT_VOL_5V00 && (BAT_Data.adc_Value > BAT_VOL_4V10))  //80~100     3359
	{
		BAT_Data.status = BAT_80_100_STATUS;
	}

}
/**************************************************************************************
* FunctionName   : uint8_t CheckWorking_Batter_State(uint16_t WorkBatValue)
* Description    : 130ms����һ��
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void CheckWorking_Batter_State(void)
{
	// static uint8_t WorkBattState = BAT_40_60_STATUS;
	static uint8_t BattState_Cnt;
	static uint16_t BattState_Cnt1;

//	if(RF_Handle.Run_Flag || SysInfo.System_Err_status == System_LowPower_Fault) //RF�����£����
	// if(SysInfo.Power_Value.state == System_ON)//if(RF_Handle.Run_Flag) //RF�����£����
	// {
	if(BAT_Data.adc_Value < BAT_VOL_3V15)    //   BAT_VOL_3V20   20230309 ���ԣ�ԭ�Ȳ⵽3.23v�ػ��������ϼ���Ҫ�� BAT_Data.
	{
		BattState_Cnt1 = 0;
		if(++BattState_Cnt > 50)
		{
			BattState_Cnt = 50;

			if(BAT_Data.status > BAT_00_00_STATUS) //低电关机百分比
			{
				BAT_Data.status = BAT_00_00_STATUS;
			}
		}
	}
	else
	{
		if(BAT_Data.status != BAT_00_20_STATUS && BAT_Data.status != BAT_00_00_STATUS)
		{
			BAT_Data.status = BAT_40_60_STATUS;
		}
		else
		{
			if(++BattState_Cnt1 > 100)
			{
				BattState_Cnt1 = 0;
				BAT_Data.status = BAT_40_60_STATUS;
			}
		}
		BattState_Cnt=0;
	}
	// }
	// else
	// {
	// 	BAT_Data.status = BAT_40_60_STATUS;
	// 	BattState_Cnt = 0;
	// }
	// return WorkBattState;
}


/**************************************************************************************
* FunctionName   : uint8_t Check_Batter_State(void)
* Description    :
* EntryParameter :
* ReturnValue    : None  1ms
**************************************************************************************/
uint8_t Scan_Batter_State(void)
{

	// static uint8_t  BatteryFlag = 1;
	// static uint16_t TimeCount;
	static uint16_t BattCount = 0;


	if(SysInfo.Montor_Flag)
	{
		BAT_Data.status = BAT_40_60_STATUS; // 开机时，马达震动显示中间档位，防止电池有点开机出现低电的情况
		return BAT_Data.status;
	}


	if(++BattCount <= BAT_ADC_COUNT)
	{
		if(!SysInfo.Montor_Flag)
		{
			Bat_ADC_Value[BattCount - 1] = filter((uint16_t *)&ADC_Value, Battery_ADC_CHANNEL, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
		}
		else
		{
			BattCount --;
		}
		return BAT_Data.status;
	}

	BattCount = 0;
	BAT_Data.adc_Value = Get_Battery_Read((uint16_t *)&Bat_ADC_Value, BAT_ADC_COUNT);

	if(dac_param.self_cal == 0x7F)  // 2023 08 03 新增，adc额外自动校准
	{
		if(BAT_Data.adc_Value > dac_param.offset)
		{
			BAT_Data.adc_Value -= dac_param.offset;
		}
	}



	// BAT_Data.adc_Value = BatValue;
	// SysInfo.Test_Mode.BatValue = BatValue;      // debug print value;
	//  printf ("\n\r bat_val =%d mv,BAT_Data.status=%d,State:%d \n\r", BAT_Data.adc_Value * 122/100, BAT_Data.status, SysInfo.Batt_Value.State);

	if(SysInfo.Power_Value.state == System_ON)  // working mode
	{
		CheckWorking_Batter_State();
		BAT_Data.bat_lvl_full_disp_delay = 0;
		BAT_Data.charge_ic_pin_full_disp_delay = 0;
	}
	else if(SysInfo.Batt_Value.Usb_flag)       // charge mode
	{
		CheckCharge_Batter_State();
	}
	else                                      //检测电量， wake up mode
	{
		BAT_Data.bat_lvl_full_disp_delay = 0;
		BAT_Data.charge_ic_pin_full_disp_delay = 0;
		CheckIdle_Batter_State();
	}

	return BAT_Data.status;
}

