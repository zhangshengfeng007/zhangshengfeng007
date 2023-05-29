#include "includes.h"
#include "Test_Mode.h"

/**************************************************************************************
 * FunctionName   : Test_Mode_Key_Process(void)
 * Description    : 测试模式按键函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/

void Test_Mode_Key_Process(void)
{
  static uint8_t Enter_Test_Flag;
  static uint16_t Test_timer_cnt, Key_Cnt;

  if (!SysInfo.Test_Mode.Quit_Flag && (SysInfo.Test_Mode.Test_Mode_Flag == OFF)) // USB插入前30s，操作进入测试模式
  {
    SysInfo.Test_Mode.Quit_Test_30s_Cnt++;
    if (++SysInfo.Test_Mode.Quit_Test_30s_Cnt >= 3000) // 超过30s无操作
    {
      SysInfo.Test_Mode.Quit_Flag = 1;
      SysInfo.Test_Mode.Quit_Test_30s_Cnt = 0;
    }
  }
  else
  {
    SysInfo.Test_Mode.Quit_Test_30s_Cnt = 0;
  }

  if (SysInfo.Batt_Value.Usb_flag && (SysInfo.Test_Mode.Test_Mode_Flag != ON))
  {
    if (0 == Enter_Test_Flag)
    {
      Test_timer_cnt++;
      if (Test_timer_cnt < 50)
      {
        if (Key_Scap_Read_In && Key_Plus_Read_In && Key_minus_Read_In)
        {
          Enter_Test_Flag = 1;
        }
      }
    }
    else if (1 == Enter_Test_Flag)
    {
      if (Key_minus_Read_In)
      {
        Enter_Test_Flag = 2;
        Test_timer_cnt = 0;
      }
    }
    else if (2 == Enter_Test_Flag)
    {
      if ((Key_Plus_Read_In == GPIO_PIN_RESET) && (Key_minus_Read_In == GPIO_PIN_RESET)) // 检测UP&DWN长按3S
      {
        Test_timer_cnt++;

        if (Test_timer_cnt > 300)
        {
          Enter_Test_Flag = 3;
          Test_timer_cnt = 0;
          SysInfo.Test_Mode.Test_Mode_Flag = Standy; // 进入测试模式
          SysInfo.Test_Mode.Test_Mode = 0;
          //					SysInfo.Power_Value.state =System_Standy;
          SysInfo.Power_Value.Enter_Sleep_Flag = 0;
          led_scan(0x00);
        }
      }
      else
      {
        Test_timer_cnt = 0;
      }
    }
    else if (3 == Enter_Test_Flag)
    {
      Test_timer_cnt++;

      if (Test_timer_cnt < 300)
      {
        if (Key_Plus_Read_In && Key_minus_Read_In) // 检测UP&DWN都松开
        {
          Enter_Test_Flag = 4;
          Test_timer_cnt = 0;
          Key_Cnt = 0;
        }
      }
      else
      {
        Enter_Test_Flag = 0x55;
      }
    }
    else if (4 == Enter_Test_Flag)
    {
      Test_timer_cnt++;

      if (Test_timer_cnt < 6000)
      {
        if (Key_Scap_Read_In == GPIO_PIN_RESET)
        {
          Key_Cnt++;
        }
        if (Key_Cnt >= 4 && Key_Scap_Read_In) // 除了短按KEY_ON，检测到其他按键退出
        {
          Enter_Test_Flag = 0x5a;
          SysInfo.Test_Mode.Test_Mode_Flag = ON; // 进入测试模式
          SysInfo.Test_Mode.Test_Mode = 0;
        }
      }
      else
      {
        Enter_Test_Flag = 0x55;
        SysInfo.Test_Mode.Test_Mode_Flag = OFF; // 退出测试模式
        ALL_LED_DOWN();
      }
    }
    else
      ;
  }
  else
  {
    Enter_Test_Flag = 0;
  }

  if (Enter_Test_Flag == 0x55) // 退出测试模式
  {
    ;
  }
}
/**************************************************************************************
 * FunctionName   : Test_LED_Display_Process(void)
 * Description    : 测试模式LED闪烁函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_LED_Display_Process(void)
{
  static uint16_t LED_Timer_Cnt;
  static uint8_t Lock_Flag, Led_Flag;

  if (Lock_Flag)
  {
    if (++LED_Timer_Cnt < 50)
    {
      Led_Flag = 1;
    }
    else
    {
      Led_Flag = 0;
      if (LED_Timer_Cnt >= 100)
      {
        Led_Flag = 0;
        LED_Timer_Cnt = 0;
      }
    }
  }

  if (SysInfo.Test_Mode.Test_Mode_Flag == Standy || SysInfo.Test_Mode.Twink_Flag == 0x04) // 模式灯闪烁
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      Mode_LED_UP();
    }
    else
    {
      Mode_LED_DOWN();
    }
  }
  else if ((SysInfo.Test_Mode.Test_Mode_Flag == ON) && (!SysInfo.Test_Mode.Test_Mode))
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      ALL_LED_UP();
    }
    else
    {
      ALL_LED_DOWN();
    }
  }
  else if (SysInfo.Test_Mode.Twink_Flag == 0x01 || SysInfo.Test_Mode.Ageing_Flag)
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      LEVEL_LED_UP();
    }
    else
    {
      LEVEL_LED_DOWN();
    }
  }
  else if (SysInfo.Test_Mode.Twink_Flag == 0x02)
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      RF_LED_UP();
    }
    else
    {
      RF_LED_DOWN();
    }
  }
  else if (SysInfo.Test_Mode.Twink_Flag == 0x03)
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      EMS_LED_UP();
    }
    else
    {
      EMS_LED_DOWN();
    }
  }
  else if (SysInfo.Test_Mode.Twink_Flag == 0x05)
  {
    Lock_Flag = 1;
    if (Led_Flag)
    {
      GEAR_LED5_UP();
      GEAR_LED4_UP();
      GEAR_LED3_UP();
      Mode_LED_UP();
    }
    else
    {
      GEAR_LED5_DOWN();
      GEAR_LED4_DOWN();
      GEAR_LED3_DOWN();
      Mode_LED_DOWN();
    }
  }
  else
    ;
}
/**************************************************************************************
 * FunctionName   : Test_RF_Vol_Process(void)
 * Description    : 测试模式RF电压驱动函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_RF_Vol_Process(void)
{
  switch (SysInfo.Test_Mode.Level)
  {
  case 0:
    led_scan(0x00);
    RF_Level_1();
    break;
  case 1:
    led_scan(0x01);
    RF_Level_1();
    break;
  case 2:
    led_scan(0x03);
    RF_Level_2();
    break;
  case 3:
    led_scan(0x07);
    RF_Level_3();
    break;
  case 4:
    led_scan(0x0f);
    RF_Level_4();
    break;
  case 5:
    led_scan(0x1f);
    RF_Level_5();
    break;
  default:
    led_scan(0x00);
    break;
  }
}
/**************************************************************************************
 * FunctionName   : Test_RF_Waveform_Process(void)
 * Description    : 测试模式RF电压驱动函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_RF_Waveform_Process(void)
{
  switch (SysInfo.Test_Mode.Level)
  {
  case 0:
    led_scan(0x00);
    RF_Level_1();
    RF_Handle.Duty = Repair_Duty0;
    break;
  case 1:
    led_scan(0x01);
    RF_Level_1();
    RF_Handle.Duty = Repair_Duty1;
    break;
  case 2:
    led_scan(0x03);
    RF_Level_2();
    RF_Handle.Duty = Repair_Duty2;
    break;
  case 3:
    led_scan(0x07);
    RF_Level_3();
    RF_Handle.Duty = Repair_Duty3;
    break;
  case 4:
    led_scan(0x0f);
    RF_Level_4();
    RF_Handle.Duty = Repair_Duty4;
    break;
  case 5:
    led_scan(0x1f);
    RF_Level_5();
    RF_Handle.Duty = Repair_Duty5;
    break;
  case 6:
    RF_Level_1();
    SysInfo.Test_Mode.Twink_Flag = 1;
    break;
  case 7:
    led_scan(0x00);
    RF_Level_5();
    RF_Handle.Duty = Repair_Duty5;
    break;
  default:
    led_scan(0x00);
    RF_Handle.Duty = Repair_Duty0;
    break;
  }
  RF_Handle.Period = RF_PERIOD;
  RF_Handle.Flag = 1;
  RF_Handle.Mode = 1;
}
/**************************************************************************************
 * FunctionName   : Test_EMS_Waveform_Process(void)
 * Description    :  测试模式EMS电压驱动函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_EMS_Waveform_Process(void)
{
  switch (SysInfo.Test_Mode.Level)
  {
  case Level_1:
    EMS_Handle.Duty = Upkeep_Duty1;
    EMS_Handle.vol_value = EMS_VOL_LEVEL1;
    led_scan(0x01);
    break;

  case Level_2:
    EMS_Handle.Duty = Upkeep_Duty2;
    EMS_Handle.vol_value = EMS_VOL_LEVEL2;
    led_scan(0x03);
    break;
  case Level_3:
    EMS_Handle.Duty = Upkeep_Duty3;
    EMS_Handle.vol_value = EMS_VOL_LEVEL3;
    led_scan(0x07);
    break;
  case Level_4:
    EMS_Handle.Duty = Upkeep_Duty4;
    EMS_Handle.vol_value = EMS_VOL_LEVEL4;
    led_scan(0x0F);
    break;
  case Level_5:
    EMS_Handle.Duty = Upkeep_Duty5;
    EMS_Handle.vol_value = EMS_VOL_LEVEL5;
    led_scan(0x1F);
    break;
  case 0x07:
    EMS_Handle.Duty = Upkeep_Duty5;
    EMS_Handle.vol_value = EMS_VOL_LEVEL5;
    led_scan(0x00);
    break;
  default:
    EMS_Handle.Duty = Upkeep_Duty0;
    EMS_Handle.vol_value = EMS_VOL_LEVEL1;
    led_scan(0x00);
    break;
  }
  EMS_Handle.Period = EMS_BOOST_SW_PERIOD;
}

/**************************************************************************************
 * FunctionName   : Test_Pole_(void)
 * Description    : 射频通道打开
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Pole_run(uint8_t Pole_Counts)
{
  switch (Pole_Counts)
  {
  case 0:
    Set_Mbi5020_Out(RF_CH1_ON_BIT);
    break;
  case 1:
    Set_Mbi5020_Out(RF_CH2_ON_BIT);
    break;
  case 2:
    Set_Mbi5020_Out(RF_CH3_ON_BIT);
    break;
  default:
    Set_Mbi5020_Out(0);
    Pole_Counts = 0;
    break;
  }
  //	return Pole_Counts;
}
/**************************************************************************************
 * FunctionName   : Test_UART_Send_Data(void)
 * Description    : 测试模式数据发送
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_UART_Send_Data(void)
{
  static uint16_t Battery_Vol;

  Battery_Vol = 2 * Get_Battery_Read((uint16_t *)&Bat_ADC_Value, BAT_ADC_COUNT) * 2500 / 4096;

  printf("\n\r ==========\n\r");
  printf("\n\r NTC_Temp=%.1f\n\r", SysInfo.NTC_Temp);
  printf("\n\r Battery_Voltage=%d\n\r", Battery_Vol);
  printf("\n\r Level=%d\n\r", SysInfo.Test_Mode.Level);
  printf("\n\r Mode=%d\n\r", SysInfo.Test_Mode.Test_Mode);
  printf("\n\r Skin_Touch=%d\n\r", SysInfo.Skin_Touch_RF_Flag);
  printf("\n\r version:%s\n\r", version);
  printf("\n\r ----------\n\r");
}
/**************************************************************************************
 * FunctionName   : uint8_t display_binary(int8_t bin_data)
 * Description    : ///二进制显示有符号整数（原码）bit6~bit0=OLED~WLED5，bit6符号位
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/

uint8_t display_binary(int8_t bin_data)
{
  uint8_t display_bin;
  uint8_t temp;

  display_bin = 0;
  temp = 0;

  if (bin_data & 0x80) // 负数
  {
    display_bin |= 0x40;      // 标记符号位
    temp = (uint8_t)bin_data; // 强制二进制转换
    temp &= 0x7f;             // 清楚符号位
    temp = (~temp) + 1;       // 求原码
    display_bin |= temp;
  }
  else
  {
    display_bin = bin_data & 0x3f;
  }

  return display_bin;
}
/**************************************************************************************
 * FunctionName   : Test_Temp_LED_Display(uint8_t display_Value)
 * Description    : 温度指示灯显示
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Temp_LED_Display(uint8_t display_Value)
{
  if (display_Value & 0x01)
  {
    GEAR_LED5_UP();
  }
  else
  {
    GEAR_LED5_DOWN();
  }

  if (display_Value & 0x02)
  {
    GEAR_LED4_UP();
  }
  else
  {
    GEAR_LED4_DOWN();
  }
  if (display_Value & 0x04)
  {
    GEAR_LED3_UP();
  }
  else
  {
    GEAR_LED3_DOWN();
  }
  if (display_Value & 0x08)
  {
    GEAR_LED2_UP();
  }
  else
  {
    GEAR_LED2_DOWN();
  }
  if (display_Value & 0x10)
  {
    GEAR_LED1_UP();
  }
  else
  {
    GEAR_LED1_DOWN();
  }
  if (display_Value & 0x20)
  {
    EMS_LED_UP();
  }
  else
  {
    EMS_LED_DOWN();
  }
  if (display_Value & 0x40)
  {
    RF_LED_UP();
  }
  else
  {
    RF_LED_DOWN();
  }
}
/**************************************************************************************
 * FunctionName   : Test_Mode_Run_Process(void)
 * Description    : 测试模式按键函数
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_Mode_Run_Process(void)
{
  static uint8_t Lock_Flag, Test_Last_Mode = 0;
#if AutoTest
  ;
#else
  static uint8_t Pole_Counts = 0, Pole_Channel_Connect, Temp_LedDisplay, Pole_Connect_Flag;
  static uint16_t Rotate_Count = 0, Skin_ADC_Value;
#endif

  if (SysInfo.Test_Mode.Test_Mode_Flag != OFF)
  {
    if (SysInfo.Test_Mode.Test_Mode == Test_Last_Mode && !SysInfo.Test_Mode.Ageing_Flag)
    {
      if (++SysInfo.Test_Mode.Quit_Test_300s_Cnt > 18000)
      {
        HAL_NVIC_SystemReset();
        SysInfo.Test_Mode.Quit_Test_300s_Cnt = 0;
      }
    }
    else
    {
      SysInfo.Test_Mode.Quit_Test_300s_Cnt = 0;
      Test_Last_Mode = SysInfo.Test_Mode.Test_Mode;
    }

#if AutoTest
    switch (SysInfo.Test_Mode.Test_Mode)
    {
    case 0: /***********************测试模式，所有指示灯闪烁***********************/
      Test_LED_Display_Process();
      Lock_Flag = 1;
      break;

    case 1: /***********************测试模式,RF驱动电压检测模式********************/
      if (Lock_Flag == 1)
      {
        RF_Handle.ADC_Flag = 1;
        RF_Handle.Flag = 1;
        RF_LED_UP();
        EMS_LED_DOWN();
        RF_Osc_ON();
        RF_Start();
        RF_Handle.Run_Flag = 1;
        Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | RF_CH3_ON_BIT);
        //					Set_Mbi5020_Out(RF_CH1_ON_BIT);
        Lock_Flag = 2;
        BOOST_5V_ON();
      }
      VBAT_DET_ON(); // 使能 V33V5020
      VBAT_OUT_ON();
      Test_RF_Waveform_Process();

      break;

    case 2: /***********************测试模式,RF波形检测模式************************/
      if (Lock_Flag == 2)
      {
        Lock_Flag = 3;
        RF_LED_UP();
        EMS_LED_DOWN();
        RF_Osc_ON();
        RF_Start();
        RF_Handle.Run_Flag = 1;
        Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | RF_CH3_ON_BIT);
        //						Set_Mbi5020_Out(RF_CH1_ON_BIT);
      }
      Test_RF_Waveform_Process();
      break;

    case 3: /***********************测试模式,EMS波形检测模式***********************/
      if (Lock_Flag == 3)
      {
        Lock_Flag = 1;
        //						Ems_Osc_On();
        EMS_Pwm_Start();
        Ems_Boost_On();
        RF_LED_DOWN();
        EMS_LED_UP();
        VBAT_DET_ON(); // 使能 V33V5020
        VBAT_OUT_ON(); //

        EMS_Handle.Run_Flag = 1;
        PID.Flag = 1;
        Set_Mbi5020_Out(EMS_CH3_ON_BIT);
#if ARF001
        RF_DeInit();
        Ems_Init();
        RF_Handle.Run_Flag = 0;
#else
        ;
#endif
      }
      Test_LED_Display_Process();
      Test_EMS_Waveform_Process();
      break;

    default:
      SysInfo.Test_Mode.Test_Mode = 0x0a;
      break;
    }

#else
    if (!SysInfo.Test_Mode.Ageing_Flag)
    {
      switch (SysInfo.Test_Mode.Test_Mode)
      {
      case 0: /***********************测试模式，所有指示灯闪烁***********************/
        Test_LED_Display_Process();
        Lock_Flag = 1;
        break;

      case 1: /***********************测试模式,RF驱动电压检测模式********************/
        if (Lock_Flag == 1)
        {
          RF_LED_UP();
          EMS_LED_DOWN();
          RF_EN_UP();
          VBAT_OUT_ON();
          Lock_Flag = 2;
        }
        Test_RF_Waveform_Process();
        Test_LED_Display_Process();
        break;
      case 2: /***********************隧道灯、电机、温度、检测模式************************/
        if (Lock_Flag == 2)
        {
          RF_LED_DOWN();
          EMS_LED_DOWN();
          IRled_start();
          Lock_Flag = 3;
          SysInfo.Test_Mode.Twink_Flag = 0X02;
        }
        Test_LED_Display_Process();

        SysInfo.Montor_Flag = 1;
        SysInfo.StayTime = 200;

        if (!SysInfo.Test_Mode.Temp_level)
        {
          EMS_LED_DOWN();
          if (SysInfo.NTC_Temp > 21 && SysInfo.NTC_Temp < 29)
          {

            Led_Value.Mode = Batt_Normal;
            Led_Value.StayTime = 0;
            Led_Value.state = 0;
            Led_Value.Level = SysInfo.Batt_Value.State;
          }
          else
          {
            Led_Value.Mode = Batt_Low_20;
            Led_Value.Level = 0x05;
          }
          Led_Display(&Led_Value);
        }
        else
        {
          Temp_LedDisplay = display_binary(SysInfo.NTC_Temp);
          Test_Temp_LED_Display(Temp_LedDisplay);
        }
        break;

      case 3: /***********************测试模式,RF波形检测模式************************/
        if (Lock_Flag == 3)
        {
          Lock_Flag = 4;
          MOTOR_OFF();
          IRled_stop();
          RF_LED_DOWN();
          EMS_LED_UP();
          RF_Osc_ON();
          RF_Start();
          RF_Handle.Run_Flag = 1;
          SysInfo.Test_Mode.Level = 0x07;
          RF_Frequency_conversion(0X02);
        }
        Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | RF_CH3_ON_BIT);
        Test_RF_Waveform_Process();

        break;

      case 4: /***********************测试模式,EMS波形检测模式***********************/
        if (Lock_Flag == 4)
        {
          Lock_Flag = 5;
          RF_Osc_Off();
          RF_Stop();
          EMS_Pwm_Start();
          Ems_Boost_On();
          VBAT_DET_ON(); // 使能 V33V5020
          VBAT_OUT_ON(); //
          SysInfo.Test_Mode.Level = 0x07;
          EMS_Handle.Run_Flag = 1;
          PID.Flag = 1;
          SysInfo.Test_Mode.Twink_Flag = 0X03;
          Set_Mbi5020_Out(EMS_CH3_ON_BIT);
        }
        Test_LED_Display_Process();
        Test_EMS_Waveform_Process();
        Set_Mbi5020_Out(EMS_CH3_ON_BIT);
        break;
      case 5: /***********************测试模式,电极导通自动检测模式***********************/
        if (Lock_Flag == 5)
        {
          Lock_Flag = 1;
          RF_Osc_ON();
          RF_Start();
          RF_Handle.Run_Flag = 1;
          SysInfo.Test_Mode.Twink_Flag = 0x04;
          Set_Mbi5020_Out(RF_CH1_ON_BIT);
          Pole_Channel_Connect = 0;
          Rotate_Count = 0;
          Pole_Counts = 0;
        }
        Test_LED_Display_Process();

        Rotate_Count++;
        if (Rotate_Count > Test_Pole_Change) // 1s
        {
          Rotate_Count = 0;
          if (++Pole_Counts > 3)
          {
            Pole_Counts = 3;
          }
        }
        Test_Pole_run(Pole_Counts);
        Skin_ADC_Value = filter((uint16_t *)&ADC_Value, 3, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
        if (Skin_ADC_Value >= TEST_RF_TOUCH_THRESHOLD)
        {
          Pole_Connect_Flag = 1;
        }
        else
        {
          Pole_Connect_Flag = 0;
        }

        if (Rotate_Count > 5 && Pole_Connect_Flag) // 变极后50ms判断电极导通
        {
          switch (Pole_Counts)
          {
          case 0:
            led_scan(0x01);
            Pole_Channel_Connect |= 0x01;
            break;
          case 1:
            led_scan(0x03);
            Pole_Channel_Connect |= 0x02;
            break;
          case 2:
            led_scan(0x07);
            Pole_Channel_Connect |= 0x04;
            break;
          default:
            break;
          }
        }
        else
        {
          if (Pole_Counts == 0x03)
          {
            if (Pole_Channel_Connect == 0x07)
            {
              led_scan(0x07);
            }
            else
            {
              SysInfo.Test_Mode.Twink_Flag = 0x05;
            }
          }
        }
        break;

      default:
        SysInfo.Test_Mode.Test_Mode = 0x0a;
        break;
      }
    }
#endif
  }
}
/**************************************************************************************
 * FunctionName   : Auto_Continuity_Test_Process(void)
 * Description    : 电极自动导通测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
uint8_t Auto_Continuity_Test_Process(void)
{
  static uint8_t AutoPole_Connect_Flag;
  static uint8_t Pole_Connect, AutoRotate_Count, AutoPole_Counts;
  static uint16_t AutoSkin_ADC_Value;
  uint8_t ReturnFlag = 0;

  AutoRotate_Count++;
  if (AutoRotate_Count > Test_Pole_Change) // 1s
  {
    AutoRotate_Count = 0;
    if (++AutoPole_Counts > 3)
    {
      AutoPole_Counts = 3;
    }
  }
  Test_Pole_run(AutoPole_Counts);
  AutoSkin_ADC_Value = filter((uint16_t *)&ADC_Value, 3, ADC_CHANNEL_NUM, ADC_ADD_COUNT);
  if (AutoSkin_ADC_Value >= TEST_RF_TOUCH_THRESHOLD)
  {
    AutoPole_Connect_Flag = 1;
  }
  else
  {
    AutoPole_Connect_Flag = 0;
  }

  if (AutoRotate_Count > 5 && AutoPole_Connect_Flag) // 变极后50ms判断电极导通
  {
    switch (AutoPole_Counts)
    {
    case 0:
      led_scan(0x01);
      Pole_Connect |= 0x01;
      break;
    case 1:
      led_scan(0x03);
      Pole_Connect |= 0x02;
      break;
    case 2:
      led_scan(0x07);
      Pole_Connect |= 0x04;
      break;
    default:
      break;
    }
  }
  else
  {
    if (AutoPole_Counts == 0x03)
    {

      //			LockFlag=1;
      if (Pole_Connect == 0x07)
      {
        led_scan(0x07);
        ReturnFlag = 1;
      }
      else
      {
        SysInfo.Test_Mode.Twink_Flag = 0x05;
        ReturnFlag = 2;
      }
      AutoPole_Counts = 0;  // 电极对
      Pole_Connect = 0;     // 电极导通数
      AutoRotate_Count = 0; // 电极通道导通计时
      //			return ReturnFlag;
    }
  }
  return ReturnFlag;
}
/**************************************************************************************
 * FunctionName   : Test_AutoMode_Run_Process(void)
 * Description    : 测试模式 自动模式测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_AutoMode_Run_Process(void)
{
  static uint8_t Montor_Flag, Cycle_Count;
  static uint8_t Test_Sleep_Flag, Get_Result;
  //	static uint16_t Auto_Mode_Cnt,Auto_Mode_Level,Pole_Level;

  if (SysInfo.Test_Mode.Test_Mode_Flag == Auto_ON)
  {
    //		printf ("\n\r=====TEST=========\n\r");
    switch (SysInfo.Test_Mode.Auto_Mode_Level)
    {
    case 0:                                               /***********************判断NTC是否正常***********************/
      if (SysInfo.NTC_Temp > 22 && SysInfo.NTC_Temp < 28) //
      {
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        //						Auto_Mode_Cnt=0;
        SysInfo.Test_Mode.Data[4] = SysInfo.NTC_Temp;
        SysInfo.Test_Mode.Send_Flag = 0x01;
      }
      else
      {
        if (++SysInfo.Test_Mode.Auto_Mode_Cnt > 500)
        {
          SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
          SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
          Montor_Flag = 1;
        }
        if (SysInfo.Test_Mode.Auto_Mode_Cnt == 100)
        {
          SysInfo.Test_Mode.Data[4] = SysInfo.NTC_Temp;
          SysInfo.Test_Mode.Send_Flag = 0x01;
        }
      }

      SysInfo.Sleep_Flag = 0;
      break;
    case 1: /***********************电极导通性测试************************/

      Get_Result = Auto_Continuity_Test_Process();
      if (Get_Result)
      {
        if (Get_Result == 0x01) // 导通
        {
          SysInfo.Test_Mode.Data[4] = 0x00;
          SysInfo.Test_Mode.Data[5] = 0x00;
        }
        else if (Get_Result == 0x02)
        {
          SysInfo.Test_Mode.Data[4] = 0xff;
          SysInfo.Test_Mode.Data[5] = 0xff;
        }
        else
          ;

        SysInfo.Test_Mode.Send_Flag = 1;
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        Get_Result = 0;
        ALL_LED_DOWN();
        SysInfo.Test_Mode.Twink_Flag = 0;
        led_scan(0x00);
        IRled_stop();
        VBAT_OUT_OFF();
      }
      break;

    case 2: /***********************RF波形检测模式***********************/
      if (!SysInfo.Test_Mode.Auto_Mode_Cnt)
      {
        RF_Osc_ON();
        RF_Start();
        RF_Handle.Run_Flag = 1;
        VBAT_DET_ON(); // 使能 V33V5020
        VBAT_OUT_ON(); //
        Set_Mbi5020_Out(RF_CH1_ON_BIT | RF_CH2_ON_BIT | RF_CH3_ON_BIT);
        Test_RF_Waveform_Process();
        //						Auto_Mode_Cnt=1;
        IRled_start();
      }
      else
      {
        if (++SysInfo.Test_Mode.Auto_Mode_Cnt > 500) // 5s时间到
        {
          //							Auto_Mode_Level =3;
          SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
          RF_Osc_Off();
          RF_Stop();
          RF_Handle.Run_Flag = 0;
        }
      }
      break;
    case 3: /***********************EMS波形检测模式***********************/
      if (!SysInfo.Test_Mode.Auto_Mode_Cnt)
      {
        EMS_Pwm_Start();
        Ems_Boost_On();
        VBAT_DET_ON(); // 使能 V33V5020
        VBAT_OUT_ON(); //
        IRled_start();
        EMS_Handle.Run_Flag = 1;
        PID.Flag = 1;
        Set_Mbi5020_Out(EMS_CH3_ON_BIT);
        Test_LED_Display_Process();
        Test_EMS_Waveform_Process();
        //						Auto_Mode_Cnt=1;
      }
      else
      {
        if (++SysInfo.Test_Mode.Auto_Mode_Cnt > 500) // 5s时间到
        {
          if (++Cycle_Count > 2)
          {
            //								Auto_Mode_Level =0x5a;
            SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
            EMS_Handle.Run_Flag = 0;
            PID.Flag = 0;
            Ems_Osc_Off();
            Ems_Boost_Off();
            led_scan(0x00);
          }
          else
          {
            //								Auto_Mode_Level =2;
            SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
          }
        }
      }
      break;
    default:
      if (++SysInfo.Test_Mode.Auto_Mode_Cnt > 15000) // 150s时间到
      {
        SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
        SysInfo.Test_Mode.Test_Mode_Flag = OFF;
        printf("\n\r=====TEST OFF=========\n\r");
        SysInfo.Test_Mode.Level = 0;
        RF_Handle.Run_Flag = 0;
        led_scan(0x00);
        ALL_LED_DOWN();
        BOOST_5V_OFF();
        VBAT_OUT_OFF();
        RF_Call_Data(); // 关闭RF、EMS
        Test_Sleep_Flag = 1;
        SysInfo.repair_level = 0;
        SysInfo.upkeep_level = 0;
        SysInfo.WorkState = System_OFF;
      }

      break;
    }
  }
  else
  {
    //		Auto_Mode_Level =0;
    //		Pole_Level=0;
    Montor_Flag = 0;
    Cycle_Count = 0;
  }

  if (Montor_Flag) // 测试不通过，电机振动5s
  {
    Montor_Flag = 0;
    SysInfo.Montor_Flag = 1; // 电机振动标志
    SysInfo.StayTime = 500;
    //		SysInfo.Test_Mode.Test_Mode_Flag=0;
  }
  if (Test_Sleep_Flag)
  {
    Test_Sleep_Flag = 0;
    //		SysInfo.Test_Mode.Sleep_Flag =1;
    SysInfo.Sleep_Flag = 1;
    SysInfo.Test_Mode.Test_Mode_Flag = 0;
    printf("\n\r=====TEST standy=========\n\r");
  }
  else
  {
    Test_Sleep_Flag = 0;
  }
  
}
/**************************************************************************************
 * FunctionName   : crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc)
 * Description    : CRC校验
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
uint16_t crc16_compute(uint8_t const *p_data, uint32_t size)
{
  uint16_t crc = 0xFFFF;

  for (uint32_t i = 0; i < size; i++)
  {
    crc = (uint8_t)(crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= (uint8_t)(crc & 0xFF) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xFF) << 4) << 1;
  }

  return crc;
}

/**************************************************************************************
 * FunctionName   : UART_Send_Data(uint8_t *Source ,uint8_t Size)
 * Description    : 数据发送
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void UART_Send_Data(uint8_t *Source, uint8_t Size)
{
  uint8_t i;
  uint16_t Test_Crc;

  Test_Crc = crc16_compute(SysInfo.Test_Mode.Data, Size - 2);
  SysInfo.Test_Mode.Data[Size - 2] = (Test_Crc >> 8) & 0xff;
  SysInfo.Test_Mode.Data[Size - 1] = Test_Crc & 0xff;

  for (i = 0; i < Size; i++)
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)(Source + i), 1, 50);
  }
}
/*************************************************************************************
 * FunctionName	 : BKP_SRAM_Init()
 * Description    :
 * EntryParameter :
 * ReturnValue    : None
 **************************************************************************************/
void BKP_SRAM_Init(void)
{
  //  TAMP->BKP0R = 0x55;
  HAL_RTCEx_BKUPWrite(&hrtc, 0, 0x55); // 备份寄存器写入0x55
  // HAL_RTCEx_BKUPRead(&hrtc,0);
}
/**************************************************************************************
 * FunctionName   : LED_Test(uint8_t Size)
 * Description    : LED灯光测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void LED_Test(uint8_t Size)
{
  ALL_LED_DOWN();
  switch (Size)
  {
  case 0:
    ALL_LED_UP();
    break;
  case 1:
    EMS_LED_UP();
    break;
  case 2:
    RF_LED_UP();
    break;
  case 3:
    IRled_start();
    break;
  case 4:
    IRled_start();
    break;
  default:
    break;
  }
}

int my_strlen(char ch[])
{
  int i = 0;
  int num = 0;
  while (ch[i] != '\0')
  {
    i++;
    num++;
  }
  return num;
}
/**************************************************************************************
 * FunctionName   : Comm_GSensor_Control_Process(void)
 * Description    : 传感器测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Comm_GSensor_Control_Process(void)
{
  if (SysInfo.Test_Mode.GSensor_Flag)
  {
    SysInfo.Test_Mode.GSensor_Flag = 0;
    VCC3V3_OUT_ON();
    Lis2dh_Init();
    LIS2DH_Data_Deal();

    SysInfo.Test_Mode.Data[1] = 0x0D;
    SysInfo.Test_Mode.Data[4] = 0x44;
    SysInfo.Test_Mode.Data[5] = (int8_t)(gdata.AXIS_X & 0xff);
    SysInfo.Test_Mode.Data[6] = (int8_t)((gdata.AXIS_X >> 8) & 0xff);
    SysInfo.Test_Mode.Data[7] = (int8_t)(gdata.AXIS_Y & 0xff);
    SysInfo.Test_Mode.Data[8] = (int8_t)((gdata.AXIS_Y >> 8) & 0xff);
    SysInfo.Test_Mode.Data[9] = (int8_t)(gdata.AXIS_Z & 0xff);
    SysInfo.Test_Mode.Data[10] = (int8_t)((gdata.AXIS_Z >> 8) & 0xff);
    SysInfo.Test_Mode.Send_Flag = 0X01;
  }
}
/**************************************************************************************
 * FunctionName   : Test_UART_Deal_Process(void)
 * Description    : 测试模式 自动模式测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Test_UART_Deal_Process(void)
{
  static uint8_t Test_Flag;
  static uint16_t Crc;
  static uint32_t Length = 0;
  //	char Test_Data[20];
  uint16_t Vbat;

  if (SysInfo.Test_Mode.Send_Flag)
  {
    SysInfo.Test_Mode.Send_Flag = 0;
    UART_Send_Data(SysInfo.Test_Mode.Data, Length);

    if (SysInfo.Test_Mode.Data[3] == 0x13 && SysInfo.Test_Mode.Data[5] == 0x00)
    {
      SysInfo.Test_Mode.Sleep_Flag = 0x01;
    }
    else if ((SysInfo.Test_Mode.Data[3] == 0x01 && SysInfo.Test_Mode.Data[5] == 0xFF) || (SysInfo.Test_Mode.Data[3] == 0x0F && SysInfo.Test_Mode.Data[5] == 0xFF))
    {
      HAL_NVIC_SystemReset();
    }
    else
      ;
    memset(SysInfo.Test_Mode.Data, 0, 20);
  }
  if (SysInfo.Receive_Flag)
  {
    SysInfo.Receive_Flag = 0;
    if (SysInfo.Test_Mode.Data[1] > 2)
    {
      Length = SysInfo.Test_Mode.Data[1];
    }
    else
    {
      Length = 2;
    }

    Crc = crc16_compute(SysInfo.Test_Mode.Data, Length - 2);
    if (SysInfo.Test_Mode.Data[Length - 2] == ((Crc >> 8) & 0xff) &&
        SysInfo.Test_Mode.Data[Length - 1] == (Crc & 0xff))
    {
      Test_Flag = 1;
      SysInfo.Test_Mode.Data[0] = 0x10;
    }
    //		UART_Send_Data(SysInfo.Test_Mode.Data,Length);//????
  }
  if (Test_Flag)
  {
    Test_Flag = 0;
    SysInfo.repair_level = 0;
    SysInfo.upkeep_level = 0;
    SysInfo.Sleep_Flag = 0;
    SysInfo.Sleep_Counts = 0;
    SysInfo.WorkState = Standy_mode;
    SysInfo.Power_Value.state = System_Standy;
    SysInfo.Power_Value.Enter_Sleep_Flag = 0;
    SysInfo.Test_Mode.Test_Mode_Flag = Auto_ON;
    SysInfo.Test_Mode.Send_Flag = 1;
    SysInfo.Test_Mode.Auto_Mode_Cnt = 0;
    BOOST_5V_ON();
    SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
    SysInfo.Test_Mode.Quit_Test_300s_Cnt = 0;

    switch (SysInfo.Test_Mode.Data[3])
    {
    case Comm_Inset_Testmode:
      if (SysInfo.Test_Mode.Data[4] == 0x00 && SysInfo.Test_Mode.Data[5] == 0x00) // 进入生产测试模式指令
      {
        SysInfo.Test_Mode.Test_Mode_Flag = Auto_ON;
        BOOST_5V_ON();
      }
      else // 退出生产测试模式指令
      {
        UART_Send_Data(SysInfo.Test_Mode.Data, Length);
        HAL_NVIC_SystemReset();
        ;
      }

      break;
    case Comm_manufacture_Info: // 获取制造信息（版本）
      Length = 0x0C;
      SysInfo.Test_Mode.Data[1] = Length;
      SysInfo.Test_Mode.Data[2] = Manufacturer_Number_H;
      SysInfo.Test_Mode.Data[3] = Manufacturer_Number_L;
      SysInfo.Test_Mode.Data[4] = Model_Number_H;
      SysInfo.Test_Mode.Data[5] = Model_Number_L;
      SysInfo.Test_Mode.Data[6] = Device_Number_H;
      SysInfo.Test_Mode.Data[7] = Device_Number_L;
      SysInfo.Test_Mode.Data[8] = Soft_Version;
      SysInfo.Test_Mode.Data[9] = Hardware_Version;
      break;
    case Comm_Ntc_Temp: // 获取NTC温度值
      SysInfo.Test_Mode.Auto_Mode_Level = 0;
      SysInfo.Test_Mode.Send_Flag = 0;
      break;
    case Comm_Batter_Value: // 获取电池电压值
      Vbat = SysInfo.Test_Mode.BatValue * 1.22;
      SysInfo.Test_Mode.Data[4] = Vbat / 1000;
      SysInfo.Test_Mode.Data[5] = (Vbat % 1000) / 10;
      break;
    case Comm_Moto_Control: // 马达连续震动
      if (SysInfo.Test_Mode.Data[4] == 0x00 && SysInfo.Test_Mode.Data[5] == 0x00)
      {
        SysInfo.Montor_Flag = 1; // 电机振动标志置1
        SysInfo.StayTime = 5000;
      }
      else
      {
        SysInfo.Montor_Flag = 0; // 电机振动标志清0
        SysInfo.StayTime = 0;
      }
      break;
    case Comm_HeadLED_Control:
      if (SysInfo.Test_Mode.Data[4] == 0x00 && SysInfo.Test_Mode.Data[5] == 0x00) // 打开灯板（LED-IR-ON）输出
      {
        IRled_start();
      }
      else // 关闭灯板（LED-IR-ON）输出
      {
        VBAT_OUT_OFF();
        IRled_stop();
      }
      break;
    case Comm_Electrode_Test: // 电极头导通性测试
      SysInfo.Test_Mode.Auto_Mode_Level = 1;
      SysInfo.Test_Mode.Send_Flag = 0;

      RF_Osc_ON();
      RF_Start();
      RF_Handle.Run_Flag = 1;
      VBAT_DET_ON(); // 使能 V33V5020
      VBAT_OUT_ON(); //
      SysInfo.Test_Mode.Level = 0x05;
      SysInfo.Test_Mode.Twink_Flag = 0x04; // 模式灯闪烁
      Set_Mbi5020_Out(RF_CH1_ON_BIT);
      Test_RF_Waveform_Process();
      break;
    case Comm_RF_Control:                                                                           // RF射频测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭RF
      {
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        SysInfo.Test_Mode.Level = 0;
        RF_Call_Data(); // 关闭RF、EMS
        led_scan(0x00);
        IRled_stop();
        VBAT_OUT_OFF();
      }
      else
      {
        SysInfo.Test_Mode.Set_RF_Freq = 2;
        SysInfo.Test_Mode.Auto_Mode_Level = 2;
        SysInfo.Test_Mode.Level = SysInfo.Test_Mode.Data[5];
        if (Length == 0x0a)
        {
          SysInfo.Test_Mode.Set_RF_Freq = 3 - SysInfo.Test_Mode.Data[7];
        }
      }
      break;
    case Comm_EMS_Control:                                                                          // 开启EMS测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭EMS
      {
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        SysInfo.Test_Mode.Level = 0;
        RF_Call_Data(); // 关闭RF、EMS
        led_scan(0x00);
        IRled_stop();
        VBAT_OUT_OFF();
      }
      else
      {
        SysInfo.Test_Mode.Set_EMS_Freq = 4;
        SysInfo.Test_Mode.Auto_Mode_Level = 3;
        SysInfo.Test_Mode.Level = SysInfo.Test_Mode.Data[5];
        if (Length == 0x0a)
        {
          SysInfo.Test_Mode.Set_EMS_Freq = SysInfo.Test_Mode.Data[7] - 1;
        }
        EMS_Frequency_conversion(SysInfo.Test_Mode.Set_EMS_Freq);
      }

      break;

    case Comm_KEY_Control:                                                                          // 进入按键测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭按键测试
      {
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        SysInfo.Test_Mode.Test_Mode_Flag = OFF;
      }
      else
      {
        RF_Call_Data(); // 关闭RF、EMS
        led_scan(0x00);
        IRled_stop();
        VBAT_OUT_OFF();
        SysInfo.Test_Mode.Send_Flag = 0;
        SysInfo.Test_Mode.Auto_Mode_Level = 0x04;
      }

      break;
    case Comm_Light_Control:                                                                        // 进入灯光测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭灯光测试
      {
        SysInfo.Test_Mode.Auto_Mode_Level = 0x5a;
        VBAT_OUT_OFF();
        IRled_stop();
        ALL_LED_DOWN();
      }
      else
      {
        LED_Test((SysInfo.Test_Mode.Data[5] - 1));
      }
      break;
    case Comm_Aging_Model:                                                                          // 老化测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭老化测试
      {
        Ageing_TestData_DeInit();
        ALL_LED_DOWN();
        //						HAL_NVIC_SystemReset();
      }
      else
      {
        SysInfo.Test_Mode.Test_Mode_Flag = ON;
        SysInfo.Test_Mode.Ageing_Flag = 1;
        SysInfo.Test_Mode.Ageing_Mode = 0;
        SysInfo.Mode_Switch_Flag = 1;
        RF_Handle.Run_Flag = 1;
        SysInfo.Skin_Touch_Flag = 1;
      }
      break;
    case Comm_Inset_Life_Testl:                                                                     // 寿命测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 关闭寿命测试
      {
        Ageing_TestData_DeInit();
        SysInfo.Test_Mode.Ageing_Flag = 0;
        SysInfo.Test_Mode.Ageing_Mode = 0;
        SysInfo.Mode_Switch_Flag = 0;
        RF_Handle.Run_Flag = 0;
        SysInfo.Skin_Touch_Flag = 0;
        SysInfo.Save_Data.save_Data_flag = 1; // 保存档位
        ALL_LED_DOWN();
        //						HAL_NVIC_SystemReset();
      }
      else
      {
        SysInfo.Test_Mode.Test_Mode_Flag = ON;
        SysInfo.Test_Mode.Ageing_Flag = 1;
        SysInfo.Test_Mode.Ageing_Mode = 1;
        SysInfo.Mode_Switch_Flag = 1;
        RF_Handle.Run_Flag = 1;
        SysInfo.Skin_Touch_Flag = 1;
        SysInfo.Save_Data.save_Data_flag = 1; // 保存档位
        RF_LED_DOWN();
        EMS_LED_UP();
        Ageing_TestData_Init();
      }
      break;

    case Comm_Inset_Bootloader: // 进入Bootloader升级
      UART_Send_Data(SysInfo.Test_Mode.Data, Length);
      BKP_SRAM_Init();
      HAL_NVIC_SystemReset();
      break;
    case Comm_Check_Version: // 版本校验

      break;
    case Comm_Standby:                                                                              // 进入待机电流测试模式
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 退出待机电流测试模式
      {
        SysInfo.Test_Mode.Sleep_Flag = 0x00;
        SysInfo.Test_Mode.Test_Mode_Flag = Auto_ON;
        SysInfo.Test_Mode.Send_Flag = 1;
        RF_Call_Data(); // 关闭RF、EMS
      }
      else
      {
        SysInfo.Test_Mode.Sleep_Flag = 0x01;
        led_scan(0x00);
        Key_S1_Long_Str();
        Key_Call_Data();
      }
      break;
    case Comm_GSensor_Control:                                                                      // G-SENSOR测试
      if (SysInfo.Test_Mode.Data[Length - 4] == 0xFF && SysInfo.Test_Mode.Data[Length - 3] == 0xFF) // 退出待机电流测试模式
      {
        Lis2dh_Sleep_on();
      }
      else
      {
        SysInfo.Test_Mode.GSensor_Flag = 0x01;
        SysInfo.Test_Mode.Send_Flag = 0;
				Length = 0x0D;
        Comm_GSensor_Control_Process();
      }
      break;

    case Comm_Quit_Testmode: // 退出测试模式
      UART_Send_Data(SysInfo.Test_Mode.Data, Length);
      HAL_NVIC_SystemReset();
      break;
    default:
      break;
    }
  }
}
/**************************************************************************************
 * FunctionName   : Test_UART_Deal_Process(void)
 * Description    : 测试模式 老化模式数据初始化
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ageing_TestData_Init(void)
{
  SysInfo.Power_Value.Enter_Sleep_Flag = 0;
  SysInfo.Power_Value.state = System_ON;
  BOOST_5V_ON();
  SysInfo.Heating_Flag = 0x00; // 开机前两秒5档运行，使电极迅速升温标志位
  SysInfo.Skin_No_Touch_Timer = 0;
  SysInfo.Freq_Cnt = 0;

  SysInfo.repair_level = Level_5;
  SysInfo.upkeep_level = Level_5;
  SysInfo.WorkState = upkeep_mode;
  SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
}
/**************************************************************************************
 * FunctionName   : Test_UART_Deal_Process(void)
 * Description    : 测试模式
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ageing_TestData_DeInit(void)
{
  SysInfo.Power_Value.Enter_Sleep_Flag = 0;
  SysInfo.Power_Value.state = System_Standy;
  BOOST_5V_OFF();
  SysInfo.Heating_Flag = 0x00; // 开机前两秒5档运行，使电极迅速升温标志位
  SysInfo.Skin_No_Touch_Timer = 0;
  SysInfo.Freq_Cnt = Frequency_1M0_CNT; // 老化模式下，RF以1mhz运行

  SysInfo.repair_level = Level_None;
  SysInfo.upkeep_level = Level_None;
  SysInfo.WorkState = Standy_mode;
  RF_Call_Data(); // 关闭RF、EMS
  IRled_stop();
}

// static uint8_t Batt_Value_State = Target_Value_State ;
/**************************************************************************************
 * FunctionName   : Ageing_Test_Process(void)
 * Description    :  老化测试
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
void Ageing_Test_Process(void)
{
  //	static uint16_t Low_Power_Cnt ;
  //	static uint16_t	AgeTimer_Cnt;

  if (SysInfo.Test_Mode.Test_Mode_Flag != OFF && SysInfo.Test_Mode.Ageing_Flag)
  {
    if (!SysInfo.Test_Mode.Ageing_Mode) // 老化测试
    {
      SysInfo.Test_Mode.Test_Mode = 0x0a;
      RF_LED_UP();
      EMS_LED_DOWN();
      Ageing_TestData_Init();

      Test_LED_Display_Process();
      if (SysInfo.Sleep_Flag)
      {
        SysInfo.Test_Mode.Ageing_Flag = 0;
        SysInfo.Test_Mode.Test_Mode_Flag = OFF;
        Test_Key_S1_Long_Str();
      }
    }
    else // 寿命测试
    {
      //			if( SysInfo.Batt_Value.State <= BAT_00_20_STATUS && !SysInfo.Test_Mode.Lock_Flag) // ???г??
      if (SysInfo.Sleep_Flag == 0x01)
      {
        SysInfo.Sleep_Flag = 0x00;
        SysInfo.Test_Mode.AgeTimer_Cnt = 0;
        SysInfo.Test_Mode.Ageing_Mode = 0x02;
        Ageing_TestData_DeInit();
      }
      else if (SysInfo.Batt_Value.State > Target_Value_State) // 充电结束，进入老化
      {
        if (++SysInfo.Test_Mode.AgeTimer_Cnt > 9000) // 90s
        {
          SysInfo.Test_Mode.AgeTimer_Cnt = 0;
          SysInfo.Test_Mode.Ageing_Mode = 0x01;
          SysInfo.Test_Mode.Lock_Flag = 0x01;
          SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State;
          //					Ageing_TestData_Init();
        }
      }
      else
      {
        if (SysInfo.Test_Mode.Lock_Flag)
        {
          if (++SysInfo.Test_Mode.AgeTimer_Cnt > 3000)
          {
            SysInfo.Test_Mode.AgeTimer_Cnt = 0;
            SysInfo.Test_Mode.Lock_Flag = 0;
          }
          SysInfo.Test_Mode.Ageing_Mode = 0x01;
          //					Ageing_TestData_Init();
        }
        else
        {
          SysInfo.Test_Mode.AgeTimer_Cnt = 0;
        }
      }

      /************************测试***************************/
      /*				if(SysInfo.Test_Mode.Ageing_Mode==0x01)
          {
              if(++AgeTimer_Cnt>3000)
              {
                  AgeTimer_Cnt = 0;
                  SysInfo.Test_Mode.Ageing_Mode=0x02;
              }
          }

          if(SysInfo.Test_Mode.Ageing_Mode==0x02)
          {
              if(++AgeTimer_Cnt>6000)
              {
                  AgeTimer_Cnt = 0;
                  SysInfo.Test_Mode.Ageing_Mode=0x01;
              }
          }
      */
      /************************测试***************************/

      if (SysInfo.Test_Mode.Ageing_Mode == 0x01)
      {

        Ageing_TestData_Init();
        ALL_LED_UP();
        RF_LED_DOWN();
        EMS_LED_UP();
        BOOST_5V_OFF();
        //				SysInfo.Test_Mode.Lock_Flag = 0x00;
        //				SysInfo.Test_Mode.Ageing_Mode=0x03;
      }
      //			else if(SysInfo.Test_Mode.Ageing_Mode==0x02)
      else
      {
        //				Ageing_TestData_DeInit();
        RF_LED_UP();
        EMS_LED_UP();
        Led_Display(&Led_Value);
        BOOST_5V_OFF();
        VBAT_OUT_OFF();
        //				SysInfo.Test_Mode.Ageing_Mode=0x03;
      }
      //			else ;
    }
  }
  else
  {
    SysInfo.Test_Mode.AgeTimer_Cnt = 0;
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
