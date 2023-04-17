#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

//volatile uint8_t Timer_1ms_Flag;
//volatile uint8_t Timer_10ms_Flag;
//volatile uint8_t Timer_100ms_Flag;

//uint32_t DMA_Transfer_Complete_Count = 0;
uint16_t Adc_Value[ADC_ADD_COUNT][ADC_CHANNLE_NUM]={0};
void ADC_DMA_COV_START(void);

_sys_Info_t       Sys_Info;
ADC_data_t        ADC_Data;


extern uint8_t buzzer_onoff_flage;

void HAL_Delay(uint32_t Delay)
{
   vTaskDelay(Delay);
}
/*************************************************************************************
* FunctionName	 : filter()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void SYSTEM_Init_Function(void)
{
  BAT_CHARGE_DISABLE;
  SYS_BUS_SW_ON;
  RGB_LED_Init();
  SENSOR_PWR_SW_ON;
  HAL_Delay(1);    //�ȴ������������ȶ�
	#ifdef COLOR_SENSOR_ENABLE
		LTR831_INIT();   //RGB��������ʼ��
	#endif
  GY6550_INIT();   //�����ǳ�ʼ��
  ADC_DMA_COV_START();
  RF_PID_Init();
  EMS_PID_Init();

	Sys_Info.Test_Mode.First_charge_Flag = 1;
	Sys_Info.Test_Mode.Test_charge_Flag = 1;
	Sys_Info.Test_Mode.RF_EMSTEST_Flag = 0;
	Sys_Info.Chargestop_Flag = 0;
  //HAL_TIM_Base_Start_IT(&htim3);
  Sys_Info.Key_State.Slide_gear      =(_gear_level_e)read_gear_for_eeprom_gear();     //��ȡ��λ  Key_State.Gears_key
  Sys_Info.Key_State.Seal_gear       =(_gear_level_e)read_gear_for_eeprom_gearseal();
  Sys_Info.Key_State.Mode_choice_key =(_choice_mode_e)read_gear_for_eeprom_mode();       //READ_MODE

  if(read_rgb_sensor_data_from_eeprom() == HAL_OK)   //RGB_Calibration_DATA_READ_FROM_FLASH
  {
    if((White_Balance.Calibration_Lux <= 1.2) || (White_Balance.Calibration_Lux >= 5))
    {
      White_Balance.Calibration_Data_Flag = 1;
    }
  }
  else
  {
    White_Balance.Calibration_Data_Flag = 1;
  }

//  if(Sys_Info.Key_State.Mode_choice_key > SLIDE_MODE)  //2022 12 30 ������ʼ����ģʽ������ ����ģʽ
//  {
//    Sys_Info.Key_State.Mode_choice_key = SEAL_MODE;
//  }
  Sys_Info.Key_State.Mode_choice_key = SLIDE_MODE; //2022 12 30 ������ʼ����ģʽ������ ����ģʽ
   Sys_Info.Key_State.Mode_key = RF_WITHOUT_EMS;           //2022 12 30  �����������ڲ���ems����Ļ���ģʽ



  if(Sys_Info.Key_State.Slide_gear > GEARTHREE)
  {
    Sys_Info.Key_State.Slide_gear = GEARTHREE;
  }
  if(Sys_Info.Key_State.Seal_gear > GEARTHREE)
  {
    Sys_Info.Key_State.Seal_gear = GEARTHREE;
  }

  RGB_Handle.Colour_Type = COLOUR_OTHER;
  Sys_Info.Work_Mode = NORMAL;
  EMS_SET_VALUE_PWM_Start(); //������ѹPWM
  RF_SET_VALUE_PWM_Start();  //������ѹPWM
}
/*************************************************************************************
* FunctionName	 : filter()
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
#define N	ADC_ADD_COUNT
uint16_t filter(uint16_t *pData, uint8_t channel, uint8_t len)
{
  uint16_t value_buf1[N] = {0};
  uint8_t count,i,j;
  uint16_t temp;

  pData = pData+channel;
  for ( count=0;count<N;count++)
  {
    value_buf1[count] =(uint16_t) *pData;
    pData = pData + len;
  }

  for (j=0;j<N-1;j++)
  {
    for (i=0;i<N-j-1;i++)
    {
      if ( value_buf1[i]>value_buf1[i+1] )
      {
        temp = value_buf1[i];
        value_buf1[i] = value_buf1[i+1];
        value_buf1[i+1] = temp;
      }
    }
  }
  return value_buf1[(N-1)/2];
}
/*************************************************************************************
* FunctionName	:
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void ADC_DMA_COV_START(void)
{
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&Adc_Value,ADC_CHANNLE_NUM*ADC_ADD_COUNT);
}
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  HAL_ADC_Stop_DMA(&hadc1);
  ADC_Data.bat_val           = filter((uint16_t *)&Adc_Value,BAT_ADC_AN,     ADC_CHANNLE_NUM);
  ADC_Data.Usb_Val            = filter((uint16_t *)&Adc_Value,USB_IN_ADC_AN,	ADC_CHANNLE_NUM);
  ADC_Data.RF_U_Val           = filter((uint16_t *)&Adc_Value,RF_U_ADC_AN,    ADC_CHANNLE_NUM);
  ADC_Data.RF_I_Val           = filter((uint16_t *)&Adc_Value,RF_I_ADC_AN,    ADC_CHANNLE_NUM);
  ADC_Data.EMS_U_Val          = filter((uint16_t *)&Adc_Value,EMS_U_ADC_AN,   ADC_CHANNLE_NUM);
  ADC_Data.RF_Temp_Val        = filter((uint16_t *)&Adc_Value,NTC_R_ADC_AN,   ADC_CHANNLE_NUM);
  ADC_Data.bat_Temp_Val      = filter((uint16_t *)&Adc_Value,BAT_TEMP_ADC_AN,ADC_CHANNLE_NUM);
  //ADC_Data.Mcu_Vref_Val   = filter((uint16_t *)&Adc_Value,MCU_VREF, ADC_CHANNLE_NUM);
  Sys_Info.Adc_First_Run_Flage = 1;
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&Adc_Value,ADC_CHANNLE_NUM*ADC_ADD_COUNT);
}

/*************************************************************************************
* FunctionName	: Charge_Insertion_Status
* Description   : ���������ʱ�Ŀ��ش���
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Charge_Insertion_Status(void)
{
  if(Sys_Info.Charge_State == CHARGING_STA)   //USB����
  {
    //static uint8_t time_cnt = 0;
    //static _sys_state_e Power_Save ;

    //if(Power_Save != Sys_Info.Power_State)
    {
      switch(Sys_Info.Power_State)
      {
      case MACHINEOFF:
        {
          if(Sys_Info.PowerOff_Time_Counter > 110)
          {
            ;
          }
          else if(Sys_Info.PowerOff_Time_Counter > 100)//��ʱ2s�ر�ϵͳ��Դ
          {
            //time_cnt = 0;
//            SENSOR_PWR_SW_OFF;
            SYS_BUS_SW_ON;
            BACKLIGHT_OFF;
            LOAD_BUS_SW_OFF;
            USB_BUS_SW_OFF;
            vTaskDelay(2);
            BAT_CHARGE_ENABLE;        // open  charge
          }
        }
        break;
      case MACHINEON:
        {
          //time_cnt = 0;
          //BAT_CHARGE_DISABLE;         // close charge
					if(Sys_Info.Work_Mode == NORMAL)
					{
						USB_BUS_SW_ON;
						SYS_BUS_SW_ON;
					}
          //HAL_Delay(2);
          //LOAD_BUS_SW_ON;
					else if(Sys_Info.Work_Mode == TESTMODE)
					{
						SYS_BUS_SW_ON;
						if(Sys_Info.Test_Mode.TestMode == Aging_TestModel)
						USB_BUS_SW_OFF;
						else if(Sys_Info.Test_Mode.TestMode == Life_TestModel)
						USB_BUS_SW_ON;
					}
        }break;
      case CHECK_BATTER:
        {
          //time_cnt = 0;
          //SYS_BUS_SW_ON;
          //LOAD_BUS_SW_OFF;
          //USB_BUS_SW_OFF;
					if(Sys_Info.Work_Mode == NORMAL)
          Sys_Info.Power_State = MACHINEOFF;
        }break;
      default:break;
      }
    }
    //Power_Save = Sys_Info.Power_State;
  }
  else                               //USBδ����
  {
    static uint8_t delay_time_cnt = 0;
		if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)//���Զ�������
		{
			switch(Sys_Info.Power_State)
			{
			case MACHINEOFF:
				{
					if(++delay_time_cnt > 100 ) //��ʱ2s�ر�ϵͳ��Դ
					{
						delay_time_cnt = 0;
						USB_BUS_SW_OFF;
						LOAD_BUS_SW_OFF;
						SYS_BUS_SW_OFF;
					}
				}
				break;
			case MACHINEON:
				{
					BAT_CHARGE_DISABLE; // close charge
					SYS_BUS_SW_ON;
					LOAD_BUS_SW_ON;
					USB_BUS_SW_OFF;
					delay_time_cnt = 0;
				}
				break;
			case CHECK_BATTER:
				{
					delay_time_cnt = 0;
					SYS_BUS_SW_ON;
					USB_BUS_SW_OFF;
					LOAD_BUS_SW_OFF;
				}
				break;
			default:break;
			}
		}
	}
}
/*************************************************************************************
* FunctionName	: Sys_Power_Handle
* Description   : ��Դ��������
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Sys_Power_On(void)
{
  //SYS_BUS_SW_ON;
  Sys_Info.Power_State = MACHINEON;
  BAT_CHARGE_DISABLE;//�ȹرճ��
  HAL_Delay(1);
  SENSOR_PWR_SW_ON;//�������ϵ�
  HAL_Delay(1); //�ȴ������������ȶ�
	#ifdef COLOR_SENSOR_ENABLE
		LTR831_INIT(); //RGB��������ʼ��
	#endif
  GY6550_INIT(); //�����ǳ�ʼ��
//	HAL_Delay(10);
  LOAD_BUS_SW_ON; //LOAD_BUS_ON
  //BACKLIGHT_ON; //��⿪��
  Sys_Info.BKlight_State = Light_Long; //���״̬
  HAL_Delay(50);
  buzzer_onoff_flage = 1;
}
/*************************************************************************************
* FunctionName	: Sys_Power_Handle
* Description   : ��Դ��������
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Sys_Power_Off(void)
{
  BACKLIGHT_OFF;
  //SENSOR_PWR_SW_OFF;
  //LOAD_BUS_SW_OFF;
  write_gear_for_eeprom(Sys_Info.Key_State.Seal_gear,\
                        Sys_Info.Key_State.Slide_gear,\
                        Sys_Info.Key_State.Mode_choice_key);///EEPROM ���ݱ���  Key_State.Gears_key

  Sys_Info.Power_State = MACHINEOFF;
}
/*************************************************************************************
* FunctionName	: loadCurrent
* Description   : ������ص����жϺ���
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
//uint16_t wactch_temp;
//const uint16_t CurrentLine[3][2] = { 700, 800,
//																		 900, 1200,
//																		 1100,1500,};  //����ģʽ�¸���λ��ͬ�ĽӴ��������
uint8_t loadCurrent(void)
{
  static uint8_t Rf_LoadStatus;
  static uint8_t Load_Count;

  if(Sys_Info.PowerOn_Time_Counter > 20)  //��������0.2S��ʼ��⸺��
  {
    if(RF_Handler.g_RfOpenFlage)
    {
      //if(ADC_Data.RF_I_Val > (RF_Handler.RF_PID.target >> 3)- 100)
      if(ADC_Data.RF_I_Val > 600)
      {
//wactch_temp = ADC_Data.RF_I_Val;
#ifdef ENABLE_CURR_PROTECT
        if(ADC_Data.RF_I_Val > 3603)  //��·��������2.3A
        {
          RF_Handler.RF_Load_Out_Scope = 1;
        }
#endif
        Load_Count = 50;
        Rf_LoadStatus = 1;
      }

      if(Load_Count > 0)
      {
        Load_Count--;
        Rf_LoadStatus = 1;
      }
      else
      {
        Rf_LoadStatus = 0;
      }
    }
  }
  else
  {
    Rf_LoadStatus = 0;
  }

  return Rf_LoadStatus;
}
/*************************************************************************************
* FunctionName	: Auto_Close_Machine
* Description   : �Զ��ػ�����
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Auto_Close_Machine(void)
{
  static uint32_t Total_contact_time = 0;//�ܽӴ�ʱ��
  static uint32_t TimeCount = 0;         //��Ƶ����ʱ��
  static uint32_t TimeSealCount = 0;     //���ظ�����Чʱ��
  static _choice_mode_e lastmode;          //��һ��ģʽ

  if(Sys_Info.Power_State == MACHINEON)
  {
    if(lastmode != Sys_Info.Key_State.Mode_choice_key)    //�ı�ģʽ�����ؼ�ʱ����
    {
      TimeCount = 0;
      Total_contact_time = 0;
    }
    lastmode = Sys_Info.Key_State.Mode_choice_key;

    if(RF_Handler.RF_Load_Flag == 0)
    {
      TimeCount ++;     //��Ƶ���ؼ�ʱ
    }
    else
    {
      if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)  //����ģʽ��ʱһ��loop��Чʱ��
      {
        if(Sys_Info.Slide_effective_flag)
        {
          Total_contact_time ++;
          TimeSealCount = 0;
        }
        else
        {
          TimeSealCount++;
        }
      }
      else
      {
        if(Sys_Info.G_sensor_active_flage)   //����ģʽ��ʱ��Ƶ���ʱ��
        {
          Total_contact_time ++;
          TimeSealCount = 0;
        }
        else
        {
          TimeSealCount++;
        }
      }
      TimeCount = 0;    //�Ӵ�Ƥ�������ؼ�ʱ����
    }

    if((TimeCount > 6000 * 2) || (TimeSealCount > 6000 * 2))    //����״̬��2���Ӻ�ػ�
    {
      TimeCount = 0; TimeSealCount = 0;
      Sys_Info.Power_State = MACHINEOFF;
    }

    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
    {
      if(Total_contact_time > 6000 * 5)           //����ģʽ�ۼ�5min��ػ�
      {
        TimeCount = 0; TimeSealCount = 0;
        Sys_Info.Power_State = MACHINEOFF;
      }
    }
    else
    {
      if(Total_contact_time > 6000 * 4)           //����ģʽ�ۼ�4min��ػ�
      {
        TimeCount = 0; TimeSealCount = 0;
        Sys_Info.Power_State = MACHINEOFF;
      }

      if(Total_contact_time == 6000 + 4000)       //��һ�λ������� 1��40��
      {
        buzzer_onoff_flage = 1;
      }

      if(Total_contact_time == (6000 + 4000) * 2) //�ڶ��λ������� 1��40��  ��ʣ��40��ػ�
      {
        buzzer_onoff_flage = 1;
      }
    }
  }
  else
  {
    Total_contact_time = 0;//�ܽӴ�ʱ��
    TimeCount = 0;         //��Ƶ����ʱ��
    TimeSealCount = 0;     //���ظ�����Чʱ��
    lastmode = SEAL_MODE;  //��һ��ģʽ
  }
}
/*************************************************************************************
* FunctionName	: AtmosphereBackLight
* Description   : ��ɫʶ��
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
#ifdef COLOR_SENSOR_ENABLE
uint8_t AtmosphereBackLight(uint8_t load)
{
  static uint8_t SamplingTimes;

  if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
  {
    if((load == 1) &&(RGB_Handle.Colour_Type == COLOUR_OTHER))//
    {
      if(SamplingTimes < 35)
      {
        SamplingTimes ++;
        Sys_Info.BKlight_State = Light_Off;
        BACKLIGHT_OFF;                    //�ر�����ͷ���
        RGB_BACKLIGHT_ON;                 //��RGB�����������
        if(SamplingTimes == 33)
        {
          RGB_Handle.Colour_Type = (_COLOUR_Enum_Typedef)ColorRecognition();       //�ж���ɫ
          if(RGB_Handle.Colour_Type == READ_ERR)
          {
            RGB_SENSER_RESET();
            SamplingTimes = 0;
            RGB_Handle.Colour_Type = COLOUR_OTHER;
          }
        }
      }
      else
      {
        //BACKLIGHT_ON;
        RGB_BACKLIGHT_OFF;
        if(Sys_Info.Temp_Protect == NoHEAT)
        {
          Sys_Info.BKlight_State = Light_Long;
        }
        else
        {
          Sys_Info.BKlight_State = Light_Twinkle;
        }
      }
    }
    else
    {
      SamplingTimes = 0;
      //BACKLIGHT_ON;
      RGB_BACKLIGHT_OFF;
      if(Sys_Info.Temp_Protect == NoHEAT)
      {
        Sys_Info.BKlight_State = Light_Long;
      }
      else
      {
        Sys_Info.BKlight_State = Light_Twinkle;
      }
    }
  }
  else
  {
    SamplingTimes = 0;
    //BACKLIGHT_ON;
    RGB_BACKLIGHT_OFF;
    RGB_Handle.Colour_Type = COLOUR_OTHER;

    if(Sys_Info.Temp_Protect == NoHEAT)
    {
      Sys_Info.BKlight_State = Light_Long;
    }
    else
    {
      Sys_Info.BKlight_State = Light_Twinkle;
    }
  }

  return RGB_Handle.Colour_Type;
}

#else
uint8_t AtmosphereBackLight(uint8_t load)
{
	return 0;
}

#endif

/***********y***************************************************************************
* FunctionName   : Get_Temp()
* Description    : ��ȡ�¶ȴ�������ֵ
* EntryParameter :
* Description    : --
* ReturnValue    : �¶�ֵ(��λ:��.)
**************************************************************************************/
const float Ka = 273.15;
const float T2 =(273.15+25.0);//T2
float Get_Temp(float Rt, float Rp ,float Bx)
{
  float temp;
  temp = Rt/Rp;
  temp = log(temp);
  temp/=Bx;
  temp+=(1/T2);
  temp = 1/(temp);
  temp-=Ka;
  return temp;
}
/**************************************************************************************
* FunctionName   : Get_Temprate()
* Description    : ��ȡSTM32�ڲ��¶ȴ�������ֵ
* EntryParameter :
* Description    : --
* ReturnValue    : �¶�ֵ(��λ:��.)
**************************************************************************************/
short Get_Mcu_Temprate(uint16_t num)
{
  short Temperature;
  Temperature = __HAL_ADC_CALC_TEMPERATURE(3000,num,ADC_RESOLUTION_12B);
  return Temperature;
}
/**************************************************************************************
* FunctionName   : TempCalculation()
* Description    : RF_NTC B3380 10K 1%  +  3.6K RES
* EntryParameter :
* Description    : --
* ReturnValue    :
**************************************************************************************/
_temp_protect_e TempCalculation(void)
{
  uint32_t RF_TEMP_RES = 0;
  static _temp_protect_e temp_return;

  RF_TEMP_RES = (uint32_t)(((double)(3600*3*4096) /(double)(ADC_Data.RF_Temp_Val *2.5))-3600) ;
  Sys_Info.Head_Temperature = (int16_t)(Get_Temp(RF_TEMP_RES, 10000,3380)); //10K,B3380

  if(Sys_Info.Head_Temperature >= Temp_Protect_Value)
  {
    temp_return = OverHEAT;
  }
  else if(Sys_Info.Head_Temperature <= (Temp_Protect_Value - 3))
  {
    temp_return = NoHEAT;
  }
  return temp_return;
}

/*************************************************************************************
* FunctionName	: Backlight_Display_Funtion
* Description   : ���״̬����
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Backlight_Display_Funtion(void)
{
  static uint8_t time_cnt = 0;

  if(RF_Handler.RF_Load_Flag)        //����ͷ�������
  {
    if(Sys_Info.Temp_Protect == NoHEAT)
    {
      B_LIGHT_LV_UP;
    }
    else
    {
      B_LIGHT_LV_DOWN;
    }
  }
  else
  {
    B_LIGHT_LV_DOWN;
  }

  if(Sys_Info.Power_State == MACHINEON)
  {
    switch(Sys_Info.BKlight_State)
    {
    case Light_Off:
      BACKLIGHT_OFF;
      break;

    case Light_Long:
      BACKLIGHT_ON;
      break;

    case Light_Twinkle:

      if(++time_cnt < 10)
      {
        BACKLIGHT_ON ;
      }
      else if(time_cnt < 15)
      {
        BACKLIGHT_OFF;
      }
      else if(time_cnt >= 15)
      {
        time_cnt = 0;
      }
      break;

    default:break;
    }
  }
  else
  {
    Sys_Info.BKlight_State = Light_Off;
  }
}
/*************************************************************************************
* FunctionName	: System_Error_Check_Funtion
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void System_Error_Check_Funtion(void)
{
  if(Sys_Info.G_sensor_state == INIT_ERROR)   //�����Ƕ�ID����
  {
    Sys_Info.Err_State.bit.gyro_error_flag = SET;
  }

  #ifdef COLOR_SENSOR_ENABLE
  if(Sys_Info.RGB_sensor_state == INIT_ERROR)    //RGB��ID����
  {
    Sys_Info.Err_State.bit.rgb_error_flag = SET;
  }
  #endif

  if(Sys_Info.Adc_First_Run_Flage)
  {
      if((ADC_Data.RF_Temp_Val > 4050) || (ADC_Data.RF_Temp_Val < 10))  //����ͷNTC��·���߶�·
      {
        Sys_Info.Err_State.bit.head_ntc_error_flag = SET;
      }
      if((ADC_Data.bat_Temp_Val > 4050) || (ADC_Data.bat_Temp_Val < 10)) // ���NTC��·���߶�·
      {
        Sys_Info.Err_State.bit.batt_ntc_error_flag = SET;
      }
      if((ADC_Data.bat_val > 4050) || (ADC_Data.bat_val < 100))       // ��ص�ѹ������
      {
        //Sys_Info.Err_State.bit.batt_value_error_flag = SET;
      }
      if((bat_data.DC5v_adc_val > DC_5400mV_ADC_VAL) || (bat_data.DC5v_adc_val < DC_4800mV_ADC_VAL)) //USB 充电异常判断
      {
        if(Sys_Info.Charge_State == CHARGING_STA)
        {
          //Sys_Info.Err_State.bit.usb_value_error_flag = SET;
        }
      }
      if(RF_Handler.RF_Load_Out_Scope)                  //����ͷ�缫��·
      {
        Sys_Info.Err_State.bit.head_short_error_falg = SET;
      }

      Sys_Info.Temp_Protect = TempCalculation();        //�¶ȼ��
  }

//  Backlight_Display_Funtion();

  if(Sys_Info.Err_State.error != 0)
  {
    Sys_Info.Work_Mode = SYS_ERROR;
  }
}
/*************************************************************************************
* FunctionName	: System_Error_Handle
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void System_Error_Handle(void)
{
  Sys_Info.Power_State = MACHINEOFF;
  EMS_OUT_Stop();
  RF_OUT_Stop();
  LOAD_BUS_SW_OFF;
  LED_Display_Error();
}



/*************************************************************************************
* FunctionName	: Control_Funtion
* Description   :
* EntryParameter:
* ReturnValue   : 10ms 调用一次
**************************************************************************************/
void Control_Funtion(void)
{
// #ifdef COLOR_SENSOR_ENABLE
	if(Sys_Info.Test_Mode.TestMode!=Auto_TestModel)
  Charge_Insertion_Status();    //USB���봦��

#ifndef CELL_TEST_MODE
	if(Sys_Info.Work_Mode == NORMAL)
  Auto_Close_Machine();         //�Զ��ػ�����,������������
#endif
  if(Sys_Info.Power_State == MACHINEON)
  {
#ifdef CELL_TEST_MODE
    Sys_Info.G_sensor_active_flage = 1;
#else
		if(Sys_Info.Work_Mode == NORMAL)
    Sys_Info.G_sensor_active_flage = AttitudeProcessing();    //6����ٶ�
		Sys_Info.G_sensor_active_flage = 1;   // 2023 01 10 ǿ�ƽ�6���־λ��1
#endif

    RGB_Handle.Colour_Type = (_COLOUR_Enum_Typedef)AtmosphereBackLight(RF_Handler.RF_Load_Flag);   //��ɫʶ��

    Sys_Info.PowerOn_Time_Counter++;
    Sys_Info.PowerOff_Time_Counter = 0;
  }
  else  //MACHINEOFF
  {
    Sys_Info.PowerOff_Time_Counter++;
    Sys_Info.PowerOn_Time_Counter = 0;
  }

  buzzerChirp(Sys_Info.Key_State.Mode_choice_key,\
              Sys_Info.Key_State.Mode_key,\
              Sys_Info.Key_State.Seal_gear,\
              Sys_Info.Key_State.Slide_gear,\
              Sys_Info.Power_State,\
              RF_Handler.RF_Load_Flag,\
              RGB_Handle.Colour_Type);//�������

}

