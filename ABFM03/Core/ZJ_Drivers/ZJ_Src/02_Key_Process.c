#include "Key_Process.h"

extern uint8_t Continue_Times;


typedef enum
{
  NO_KEY_MSG,

  POWER_KEY_PRESS_MSG,
  POWER_KEY_UP_MSG,
  POWER_KEY_LONG_MSG,

  LEVEL_ADD_KEY_PRESS_MSG,
  LEVEL_ADD_KEY_UP_MSG,
  LEVEL_ADD_KEY_LONG_MSG,

  LEVEL_DEC_KEY_PRESS_MSG,
  LEVEL_DEC_KEY_UP_MSG,
  LEVEL_DEC_KEY_LONG_MSG,
}Key_MSG_e;


typedef struct
{
  _Key_Input  CurKey;
  _Key_Input  Last_Key;
  Key_MSG_e   key_msg;
}struct_KeyInfo;

_Key_Input Key_Scan(void);

static struct_KeyInfo g_KeyInfo1 = {KEY_None, KEY_None, NO_KEY_MSG};
static struct_KeyInfo Test_KeyInfo1 = {KEY_None, KEY_None, NO_KEY_MSG};


/**************************************************************************************
* FunctionName   : Key_Call_Data
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Key_Call_Data(void)
{
  // SysInfo.Save_Data.save_mode_count = 0;
	SysInfo.Idle_Run_Cnt=0;
}


/**************************************************************************************
* FunctionName   : Key_Call_Data
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Sys_Wakeup_init(void)
{
  //VBAT_OUT_ON(); //使能ADC电压采集
  // BOOST_5V_ON();
  BOOST_3V3_ON();
  HAL_SPI_MspInit(&hspi1);
  SysInfo.Batt_Value.Power_Display_Flag = 1; // 电量显示标志
  SysInfo.Power_Value.BattState = 0;
  SysInfo.Power_Value.state = System_OFF;
  SysInfo.Sleep_Counts = 0;
  Led_Value.StayTime = 200;
  RGB_Init();

}

/**************************************************************************************
* FunctionName   : Dealwith_power_key_press()
* Description    : 按键按下时
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_power_key_press(void) //
{

  BOOST_3V3_ON();   // 先将3.3V供电部分打开，使Vref 电压正常---->ADC读取正常

}


/**************************************************************************************
* FunctionName   : Dealwith_power_key_up
* Description    : 开机状态下，短按不动作，关机状态下，短按查询电量
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_power_key_up(void) //
{

	switch(SysInfo.Power_Value.state)
	{
		case System_ON:
		case System_Standy:
		case System_SHORT_CIRCUIT_PROTECT:
		{
			if(++SysInfo.WorkState>upkeep_mode)
			{
				Ems_DeInit();
				SysInfo.WorkState= repair_mode;
			}
			SysInfo.Montor_Flag =1; //使能电机振动标志
			SysInfo.StayTime = 20;
			SysInfo.Skin_Touch_Montor_Flag =1; //RF开启，第一次接触到皮肤，电机振动标志位
			SysInfo.Power_Value.state = System_ON ;
			SysInfo.Idle_Run_Cnt=0;
			SysInfo.Save_Data.WorkState = SysInfo.WorkState;
			SysInfo.Skin_Touch_Flag=0;
			SysInfo.Skin_Touch_RF_Flag = 0;
			SysInfo.Freq_Cnt=0;
			if(SysInfo.Power_Value.state == System_Standy) //使用完一个模式后，切换另外一个模式
			{
				SysInfo.Heating_Flag =0x01; //开机前两秒5档运行，使电极迅速升温标志位
			}
			SysInfo.Mode_Switch_Flag = 0x01 ; //RF模式开启
			SysInfo.Reminder_Cnt=0;
			IRled_start();

      // if( hw_2_4G_data.mode == AUTO_MATCH_MODE)
      // {
      //      hw_2_4G_data.mode = NORMAL_MODE;
      // }
      // else
      // {
      //    hw_2_4G_data.mode = AUTO_MATCH_MODE;
      // }
			break;
		}
		case System_OFF:
		{
      Sys_Wakeup_init();
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : Dealwith_power_key_long
* Description    : 长按开关机  BAT_30_40_STATUS  123
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_power_key_long(void)
{
//	uint8_t Flag=0;
	if(SysInfo.Power_Value.state == System_OFF)
	{
		if((SysInfo.Batt_Value.State == BAT_00_00_STATUS) ||
        ((SysInfo.Save_Data.BattState == BAT_00_00_STATUS) && (SysInfo.Batt_Value.State < BAT_30_40_STATUS)))
    // if(SysInfo.Batt_Value.State == BAT_00_00_STATUS)
		{
		  Sys_Wakeup_init();
		  Led_Value.Mode = Batt_Low_0 ; //缺电，1档LED灯1Hz频率闪烁5次
		  Led_Value.StayTime = 400;
		}
		else
		{
			VBAT_OUT_ON(); //使能ADC电压采集
			BOOST_5V_ON();
			BOOST_3V3_ON();

			Read_Current_Level_For_Eeprom();
			SysInfo.Mode_Switch_Flag = 0x01 ; //RF模式开启

			SysInfo.Power_Value.Enter_Sleep_Flag =0;
			SysInfo.Power_Value.state = System_ON ;
			SysInfo.WorkState= SysInfo.Save_Data.WorkState;

			SysInfo.Skin_Touch_Montor_Flag =1; //RF开启，第一次接触到皮肤，电机振动标志位
			SysInfo.Heating_Flag =0x01;        //开机前两秒5档运行，使电极迅速升温标志位
			SysInfo.Idle_Run_Cnt=0;
			SysInfo.Freq_Cnt=0;

			//			Mode_led_scan(SysInfo.WorkState);
			// HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);     //关闭外部中断POWER_KEY_PIN使能   ---占用4个字节
			hw2000b_init_250k();
			GY6550_INIT();   //陀螺仪初始化
			RGB_Init();
			MX_USART2_UART_Init();
			sys_swd_clk_init();
			Ems_dac_probe();
      ReadDevice_ID();
      IRled_start();
			SysInfo.MotionStateFlage = 0x00;
			SysInfo.Lock_State  = Lock_ON;
			SysInfo.Batt_Value.Power_Display_Flag = 0;
      hw_2_4G_data.mode = NORMAL_MODE;
      SysInfo.Montor_Flag = 1; //电机振动标志
			SysInfo.StayTime = 20;
      printf ("\n\r key long _ inside_ver: %s \n\r", inside_version);
		}
		SysInfo.StayTime_30s_Flag = 1; //切换档位后,在该档位维持30s,避免因温度过高，导致切换档位一直维持1档运行

    //		printf ("\n\r repair_level=%d\n\r",SysInfo.Save_Data.repair_level);//测试
    //		printf ("\n\r upkeep_level=%d\n\r",SysInfo.Save_Data.upkeep_level);//测试
	}
	else
	{
		//Level_led_scan(LEVEL_NULL_DISP);
		//Mode_led_scan(SysInfo.WorkState);
		//State_led_scan(BLACK);
		RF_Osc_Off();
		RF_Stop();
		Set_Mbi5020_Out(CLOSE_RF_EMS_OUT_BIT);
		//IRled_stop();
    Led_Value.Mode = SLEEP_MODE_DISP;

		SysInfo.Power_Value.state = System_OFF;
		SysInfo.Save_Data.save_Data_flag = 1;
		SysInfo.Save_Data.BattState = SysInfo.Batt_Value.State ;
		SysInfo.WorkState= Standy_mode;
		SysInfo.OverTemp_Flag =0;
		SysInfo.StayTime_30s_Flag =0;
		SysInfo.Skin_Touch_Flag = 0;
		SysInfo.Skin_Touch_RF_Flag = 0;
		// SysInfo.Flag = 0x01;
		SysInfo.Test_Mode.EMS_Flag = 0;
		SysInfo.Test_Mode.RF_Flag = 0;

//		VBAT_OUT_OFF() ;
		Key_Call_Data();
		SysInfo.Montor_Flag =1; //电机振动标志
		if(!SysInfo.Sleep_Flag)
		{
			SysInfo.StayTime = 20;
		}
		//Led_Value.Level=0x5a;
	  SysInfo.Power_Value.Enter_Sleep_Flag = 1;
		SysInfo.System_Err_status = System_Normal;
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn); //使能POWER_KEY_PIN中断
		SysInfo.Reminder_Cnt=0;
		HAL_NVIC_DisableIRQ(EXTI4_15_IRQn); // 关闭 无线中断脚
    HW_CE_LOW();

//		SysInfo.Test_Mode.RF_Flag =0x00;
//		SysInfo.Test_Mode.EMS_Flag =0x00;
	}
	SysInfo.Sleep_Counts =0;

}

/**************************************************************************************
* FunctionName   : Dealwith_level_add_key_up
* Description    : 短按 加档 最大加到5档
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_level_add_key_up(void)  //加档
{
  switch(SysInfo.WorkState)
  {
    case repair_mode:
    {
      SysInfo.repair_level	=	SysInfo.Save_Data.repair_level; //温度过高，会降到1档
  		if(SysInfo.repair_level < Level_3)
  		{
  			SysInfo.repair_level ++;
  		}
  		SysInfo.Save_Data.repair_level = SysInfo.repair_level;
      break;
    }
    case upkeep_mode:
    {
  		SysInfo.upkeep_level  = SysInfo.Save_Data.upkeep_level;

  		if(SysInfo.upkeep_level < Level_3)
  		{
  			SysInfo.upkeep_level ++;
  		}
  		Ems_Value_Add(&EMS_Handle);
  		SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
      break;
    }

    default:
    {
      break;
    }
  }
	SysInfo.Idle_Run_Cnt = 0;
	SysInfo.StayTime_30s_Flag = 1; //切换档位后,在该档位维持30s,避免因温度过高，导致切换档位一直维持1档运行
  SysInfo.Save_Data.save_Data_flag = 1;


}


/**************************************************************************************
* FunctionName   : Dealwith_level_add_key_long
* Description    :
* EntryParameter :
* ReturnValue    : 长按上按键---底座解锁功能
**************************************************************************************/
void Dealwith_level_add_key_long(void)
{
	if(SysInfo.Power_Value.state == System_ON)
	{
    if(IS_LEVEL_DEC_KEY_UP())
    {
      	SysInfo.Lock_State  = Lock_ON;
		    SysInfo.Montor_Flag = 1; //电机振动标志
	  	  SysInfo.StayTime = 20;

    }

	}
}
/**************************************************************************************
* FunctionName   : Dealwith_level_dec_key_up
* Description    : 短按 减档 最小减到1档
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_level_dec_key_up(void)  //减档
{

	// if(SysInfo.WorkState== repair_mode)
	// {
	// 	 SysInfo.repair_level	=	SysInfo.Save_Data.repair_level  ; //温度过高，会降到1档
	// 	if(--SysInfo.repair_level<Level_1)
	// 	{
	// 		SysInfo.repair_level = Level_1 ;
	// 	}
	// 		SysInfo.Save_Data.repair_level = SysInfo.repair_level ;
	// }
	// else if(SysInfo.WorkState== upkeep_mode)
	// {
	// 	SysInfo.upkeep_level  = SysInfo.Save_Data.upkeep_level;
	// 	if(--SysInfo.upkeep_level<Level_1)
	// 	{
	// 		SysInfo.upkeep_level = Level_1 ;
	// 	}
	// 	Ems_Value_Reduce(&EMS_Handle);
	// 	SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level ;
	// }

  switch (SysInfo.WorkState)
  {
    case repair_mode:
    {
        SysInfo.repair_level	=	SysInfo.Save_Data.repair_level; //温度过高，会降到1档
        if(SysInfo.repair_level > Level_1)
        {
          SysInfo.repair_level --;
        }
        SysInfo.Save_Data.repair_level = SysInfo.repair_level;
        break;
    }
    case upkeep_mode:
    {
    		SysInfo.upkeep_level  = SysInfo.Save_Data.upkeep_level;
    		if(SysInfo.upkeep_level > Level_1)
    		{
    			SysInfo.upkeep_level --;
    		}
    		Ems_Value_Reduce(&EMS_Handle);
    		SysInfo.Save_Data.upkeep_level = SysInfo.upkeep_level;
        break;
    }
    default:
    {
      break;
    }
  }
	SysInfo.Idle_Run_Cnt = 0;
	SysInfo.StayTime_30s_Flag = 1; //切换档位后,在该档位维持30s,避免因温度过高，导致切换档位一直维持1档运行
  SysInfo.Save_Data.save_Data_flag = 1;


}

/**************************************************************************************
* FunctionName   : Dealwith_level_dec_key_long
* Description    :
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_level_dec_key_long(void)
{

}

/**************************************************************************************
* FunctionName   : Dealwith_testmode_power_key_up
* Description    : 测试模式下，
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_testmode_power_key_up(void) //
{
  if((AGING_TEST == SysInfo.Test_Mode.STATUS)||(INSET_LIFE_TEST == SysInfo.Test_Mode.STATUS))
  {
    return;
  }

  if((KEY_TEST == SysInfo.Test_Mode.STATUS))
  {
    dealwith_KEY_test_power_key_up();
    return;
  }

	if(++SysInfo.Test_Mode.STATUS > READ_NTC_TEST)//7)
	{
		SysInfo.Test_Mode.STATUS = TILA_MODE_RF_TEST;
	}
//	SysInfo.Test_Mode.Level =1;
//	SysInfo.Test_Mode.Twink_Flag =0;
//	RF_Call_Data();
	SysInfo.Reminder_Cnt = 0; // 20230320 ，修改测试模式下，按键动作，将运行时长累计时长清0,同正常使用模式
	SysInfo.Montor_Flag =1; //使能电机振动标志
	SysInfo.StayTime = 50;
	Key_ctrl_test_mode_func();
}

/**************************************************************************************
* FunctionName   : Dealwith_power_key_long
* Description    : 长按退出测试模式
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_testmode_power_key_long(void)
{
	SysInfo.Test_Mode.Test_Mode_Flag = OFF; //退出测试模式
	SysInfo.Test_Mode.STATUS =0;
	Dealwith_power_key_long();
	HAL_NVIC_SystemReset();
}


/**************************************************************************************
* FunctionName   : Dealwith_testmode_power_key_up
* Description    : 老化模式
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_testmode_level_add_key_up(void) //
{
  if((AGING_TEST == SysInfo.Test_Mode.STATUS)||(INSET_LIFE_TEST == SysInfo.Test_Mode.STATUS))
  {
    return;
  }

  if((KEY_TEST == SysInfo.Test_Mode.STATUS))
  {
    dealwith_KEY_test_level_add_key_up();
    return;
  }

	  Dealwith_level_add_key_up();
}

/**************************************************************************************
* FunctionName   : Dealwith_power_key_long
* Description    : 长按进入老化测试模式
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_testmode_level_add_key_long(void)
{
  if((AGING_TEST == SysInfo.Test_Mode.STATUS)||(INSET_LIFE_TEST == SysInfo.Test_Mode.STATUS)||(NONE_TEST == SysInfo.Test_Mode.STATUS))
  {
    SysInfo.Montor_Flag = 1; //电机振动标志
    SysInfo.StayTime = 40;

    if(AGING_TEST != SysInfo.Test_Mode.STATUS)
    {
      enter_agine_test_init();
    }
    else
    {
      enter_inset_life_test_init();
    }
    return;
  }

	  Dealwith_level_add_key_long();
}

/**************************************************************************************
* FunctionName   : Dealwith_testmode_power_key_up
* Description    : 档位
* EntryParameter :
* ReturnValue    :
**************************************************************************************/
void Dealwith_testmode_level_dec_up(void) //
{
  if((AGING_TEST == SysInfo.Test_Mode.STATUS)||(INSET_LIFE_TEST == SysInfo.Test_Mode.STATUS))
  {
    return;
  }
  if((KEY_TEST == SysInfo.Test_Mode.STATUS))
  {
    dealwith_KEY_test_level_dec_key_up();
    return;
  }
	Dealwith_level_dec_key_up();
}

/**************************************************************************************
* FunctionName   : Dealwith_testmode_dec_key_long
* Description    : 档位
* EntryParameter :
* ReturnValue    :   //  长按档位减按键，测试模式长按--减，进入老化模式，再次长按档位减，进入寿命测试
**************************************************************************************/
void Dealwith_testmode_level_dec_long(void) //
{
    SysInfo.Montor_Flag = 1; //电机振动标志
		SysInfo.StayTime = 40;

    if(AGING_TEST != SysInfo.Test_Mode.STATUS)
    {
      enter_agine_test_init();
    }
    else
    {
      enter_inset_life_test_init();
    }
}


/************************************测试模式按键操作*********************************/



/**************************************************************************************
* FunctionName   : get_key_msg()
* Description    :
* EntryParameter : None
* ReturnValue    : None  10ms调用一次
**************************************************************************************/
static void get_key_msg(struct_KeyInfo* pInfo)
{
  static uint8_t power_key_press_cnt;
  static uint8_t level_add_key_press_cnt;
  static uint8_t level_dec_key_press_cnt;

  switch(pInfo->CurKey)
  {
    case POWER_KEY_Press:
    {
      power_key_press_cnt ++;

      if(power_key_press_cnt == 1) // 按键按下后，就发出 key_press_msg
      {
        pInfo->Last_Key = POWER_KEY_Press;
        pInfo->key_msg = POWER_KEY_PRESS_MSG;
        BOOST_3V3_ON();   // 先将3.3V供电部分打开，使Vref 电压正常---->ADC读取正常
      }

      if(power_key_press_cnt == 200)
      {
        pInfo->key_msg = POWER_KEY_LONG_MSG;
      }
      break;
    }
    case LEVEL_ADD_KEY_Press:
    {
      level_add_key_press_cnt ++;
      pInfo->Last_Key = LEVEL_ADD_KEY_Press;

      if(level_add_key_press_cnt == 200)
      {
        pInfo->key_msg = LEVEL_ADD_KEY_LONG_MSG;
      }
      break;
    }
    case LEVEL_DEC_KEY_Press:
    {
      level_dec_key_press_cnt ++;
      pInfo->Last_Key = LEVEL_DEC_KEY_Press;
      if(level_dec_key_press_cnt == 200)
      {
        pInfo->key_msg = LEVEL_DEC_KEY_LONG_MSG;
      }
      break;
    }


    case KEY_None:
    default:
    {
      switch(pInfo->Last_Key)
      {

        case POWER_KEY_Press:
        {
            if((System_OFF == SysInfo.Power_Value.state)&&((power_key_press_cnt <= 10)||(power_key_press_cnt >= 50)&&(power_key_press_cnt < 200)))
            {
                BOOST_3V3_OFF();
            }
            if((power_key_press_cnt > 10)&&(power_key_press_cnt < 50))
            {
              pInfo->key_msg = POWER_KEY_UP_MSG;
            }
            break;
        }
        case LEVEL_ADD_KEY_Press:
        {
            if((level_add_key_press_cnt > 10)&&(level_add_key_press_cnt < 50))
            {
              pInfo->key_msg = LEVEL_ADD_KEY_UP_MSG;
            }
            break;
        }
        case LEVEL_DEC_KEY_Press:
        {
            if((level_dec_key_press_cnt > 10)&&(level_dec_key_press_cnt < 50))
            {
              pInfo->key_msg = LEVEL_DEC_KEY_UP_MSG;
            }
            break;
        }
        default:
        {
            pInfo->key_msg = NO_KEY_MSG;
            break;
        }
      }
      power_key_press_cnt = 0;
      level_add_key_press_cnt = 0;
      level_dec_key_press_cnt = 0;
      pInfo->Last_Key = KEY_None;
      break;
    }
  }
}

/**************************************************************************************
* FunctionName   : dealwith_key_msg()
* Description    :
* EntryParameter : None
* ReturnValue    : None  10ms调用一次
**************************************************************************************/
static void dealwith_key_msg(struct_KeyInfo* pInfo)
{
    switch(pInfo->key_msg)
    {
        case POWER_KEY_PRESS_MSG:
        {
            Dealwith_power_key_press();
            break;
        }
        case POWER_KEY_UP_MSG:
        {
            Dealwith_power_key_up();
            break;
        }
        case POWER_KEY_LONG_MSG:
        {
            Dealwith_power_key_long();
            break;
        }
        case LEVEL_ADD_KEY_UP_MSG:
        {
            Dealwith_level_add_key_up();
            break;
        }
        case LEVEL_ADD_KEY_LONG_MSG:
        {
            Dealwith_level_add_key_long();
            break;
        }

        case LEVEL_DEC_KEY_UP_MSG:
        {
            Dealwith_level_dec_key_up();
            break;
        }
        case LEVEL_DEC_KEY_LONG_MSG:
        {

            break;
        }

        case NO_KEY_MSG:
        default:
        {
            break;
        }
    }
    pInfo->key_msg = NO_KEY_MSG;
}


/**************************************************************************************
* FunctionName   : dealwith_Test_key_msg()
* Description    :
* EntryParameter : None
* ReturnValue    : None  10ms调用一次
**************************************************************************************/
static void dealwith_Test_key_msg(struct_KeyInfo* pInfo)
{
    switch(pInfo->key_msg)
    {
        case POWER_KEY_UP_MSG:
        {
            Dealwith_testmode_power_key_up();
            break;
        }
        case POWER_KEY_LONG_MSG:
        {
            Dealwith_testmode_power_key_long();
            break;
        }

        case LEVEL_ADD_KEY_UP_MSG:      //----------level_add
        {
            Dealwith_testmode_level_add_key_up();
            break;
        }
        case LEVEL_ADD_KEY_LONG_MSG:
        {
            Dealwith_testmode_level_add_key_long();
            break;
        }

        case LEVEL_DEC_KEY_UP_MSG:
        {

            Dealwith_testmode_level_dec_up();
            break;
        }
        case LEVEL_DEC_KEY_LONG_MSG:
        {
            Dealwith_testmode_level_dec_long();
            break;
        }

        case NO_KEY_MSG:
        default:
        {
            break;
        }
    }
    pInfo->key_msg = NO_KEY_MSG;
}


/**************************************************************************************
* FunctionName   : Key_Proc()
* Description    :
* EntryParameter : None
* ReturnValue    : None  10ms调用一次
**************************************************************************************/
void Key_Proc(void)
{
  g_KeyInfo1.CurKey = Key_Scan();
  get_key_msg(&g_KeyInfo1);
  dealwith_key_msg(&g_KeyInfo1);
}

/**************************************************************************************
* FunctionName   : Test_Key_Proc()
* Description    :
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void Test_Key_Proc(void)
{
  Test_KeyInfo1.CurKey = Key_Scan();
  get_key_msg(&Test_KeyInfo1);
  dealwith_Test_key_msg(&Test_KeyInfo1);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
