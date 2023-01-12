#include "includes.h"

extern SPI_HandleTypeDef hspi2;

uint8_t Led_Colour[9] = {0};//G R B 
uint8_t LCD_data[73];

void LCD_output_transf(uint8_t *data);
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void RGB_LED_Init(void)
{
  uint8_t i;
  for(i=0 ; i<sizeof(Led_Colour); i++)
  {
    Led_Colour[i] = 0x00;
  }
  LCD_output_transf(Led_Colour);
}
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void LCD_output_transf(uint8_t *data)
{
  uint8_t i,j;
  
  for(i = 0;i < 9;i++)
  {
    for(j = 0;j < 8;j++)
    {
      if((data[i] & 0x80)==0x80)
      {
        LCD_data[8*i + j] = CodeOne;
      }
      else
      {
        LCD_data[8*i + j] = CodeZero;
      }
      data[i] = data[i] << 1;
    }
  }
  
  LCD_data[72] = 0;
  
  HAL_SPI_Transmit_DMA(&hspi2,LCD_data,sizeof(LCD_data));	
}
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Set_Color_Handle(uint8_t R,uint8_t G,uint8_t B,uint8_t num)
{
  Led_Colour[num *3+ 0] = G;
  Led_Colour[num *3+ 1] = R;
  Led_Colour[num *3+ 2] = B;
	
//	Led_Colour[num *3+ 0] = R;
//	Led_Colour[num *3+ 1] = G;
//	Led_Colour[num *3+ 2] = B;
}
/*************************************************************************************
* FunctionName	:
* Description		:
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Multicolor_Lamp(uint8_t colour)
{
  switch(colour)
  {
  case COLOUR_OTHER: Set_Color_Handle(0,0,0,0);       break;
  case COLOUR_ORANGE:Set_Color_Handle(255,80,0,0);    break;
  case COLOUR_BLUE:  Set_Color_Handle(30,160,255,0);  break;			
  case COLOUR_GREEN: Set_Color_Handle(0,255,0,0);     break;
  case COLOUR_BROWN: Set_Color_Handle(200,128,100,0); break;
  default : Set_Color_Handle(0,0,0,0); break;
  }
}
/*************************************************************************************
* FunctionName	: Display_Batter_Charge
* Description		: 充电指示
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Display_Batter_Charge(uint8_t level)
{
  static uint8_t time_cnt = 0;
  
  if(++time_cnt < 10)
  {
    switch(level)
    {
    case Batt_Level_Gear_0: LED1_ON; LED2_OFF;LED3_OFF; break;
    case Batt_Level_Gear_1: LED1_ON; LED2_OFF;LED3_OFF; break;
    case Batt_Level_Gear_2: LED1_ON; LED2_ON; LED3_OFF; break;
    case Batt_Level_Gear_3: LED1_ON; LED2_ON; LED3_ON;  break;
    case Batt_Level_SP20  : LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_SP10  : LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_Full  : LED1_ON; LED2_ON; LED3_ON;  break;
    default:break;  
    }
  }
  else if(time_cnt < 20)
  {
    
    switch(level)
    {
    case Batt_Level_Gear_0: LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_Gear_1: LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_Gear_2: LED1_ON ;LED2_OFF;LED3_OFF; break;
    case Batt_Level_Gear_3: LED1_ON ;LED2_ON; LED3_OFF; break;
    case Batt_Level_SP20  : LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_SP10  : LED1_OFF;LED2_OFF;LED3_OFF; break;
    case Batt_Level_Full  : LED1_ON; LED2_ON; LED3_ON;  break;
    default:break;  
    }
  }
  else
  {
    time_cnt = 0;
  }
}
/*************************************************************************************
* FunctionName	: void Display_Batter_Level(uint8_t level)
* Description		: 电量查看显示
* EntryParameter:
* ReturnValue		:
**************************************************************************************/
void Display_Batter_Level(uint8_t level)
{

		switch(level)
		{
		case Batt_Level_Gear_0: LED1_ON;LED2_OFF;LED3_OFF; break;
		case Batt_Level_Gear_1: LED1_ON;LED2_OFF;LED3_OFF; break;
		case Batt_Level_Gear_2: LED1_ON;LED2_ON;LED3_OFF; break;
		case Batt_Level_Gear_3: LED1_ON;LED2_ON;LED3_ON; break;
		case Batt_Level_SP20  :
		case Batt_Level_SP10  :
		case Batt_Level_Full  :
		default:break;  

	}
}
/*************************************************************************************
* FunctionName	: Display_Batter_Usering 
* Description   : 使用过程中的低电提示
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void Display_Batter_Usering(uint8_t level)
{
  static uint8_t time_cnt = 0;
  static uint8_t lowpower_autooff_cnt = 0;
  
  if(++time_cnt < 10)
  {
    switch(level)
    {
    case Batt_Level_Gear_0: break;
    case Batt_Level_Gear_1: break;
    case Batt_Level_Gear_2: break;
    case Batt_Level_Gear_3: break;
    case Batt_Level_SP20  : LED1_OFF;LED2_OFF;LED3_OFF;break;
    case Batt_Level_SP10  : LED1_OFF;LED2_OFF;LED3_OFF;lowpower_autooff_cnt++;break;
    case Batt_Level_Full  :
    default:break;  
    }
  }
  else if(time_cnt < 20)
  {
    ;
  }
  else
  {
    time_cnt = 0;
  }
  if(Sys_Info.Work_Mode != TESTMODE)//老化测试模式低电不关机
	{
		if(lowpower_autooff_cnt > 30) //低电量闪灯3次后自动关机
		{
			lowpower_autooff_cnt = 0;
			Sys_Info.Power_State = MACHINEOFF;
		}
	}
}

void Display_LED_Gear(uint8_t level)
{
  switch(level) //档位显示
  {
  case GEARONE  :LED1_ON;LED2_OFF;LED3_OFF;break;
  case GEARTWO  :LED1_ON;LED2_ON;LED3_OFF;break;
  case GEARTHREE:LED1_ON;LED2_ON;LED3_ON;break;
  default:break;
  }
}
/*************************************************************************************
* FunctionName	: LED_display  LED燈服務函數，100ms刷新一次
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void LED_display(void)
{
  static uint8_t time_cnt = 0;
  if(Sys_Info.Power_State == MACHINEON) //开机状态
  {
    if(Sys_Info.Key_State.Mode_choice_key == SEAL_MODE)
    {
      Set_Color_Handle(255,255,255,1);
      Set_Color_Handle(0,0,0,2);
      
      Display_LED_Gear(Sys_Info.Key_State.Seal_gear); //档位显示
    }
    else if(Sys_Info.Key_State.Mode_choice_key == SLIDE_MODE)
    {
      Set_Color_Handle(0,0,0,1);
      if(Sys_Info.Key_State.Mode_key == RF_WITH_EMS)
      {
				if(Sys_Info.Work_Mode == NORMAL)
        Set_Color_Handle(255,255,255,2);
				else if(Sys_Info.Work_Mode == TESTMODE)
				Set_Color_Handle(0,255,255,2);//(0,0,0,2);
      }
      else
      {
        Set_Color_Handle(0,255,255,2);
      }
      
      Display_LED_Gear(Sys_Info.Key_State.Slide_gear);  //档位显示
    }
    
		if(Sys_Info.Test_Mode.TestMode != Aging_TestModel)//老化模式不显示颜色校准报警
		{
			if(White_Balance.Calibration_Data_Flag) //没有RGB校准信息 警告
			{
				static uint8_t err_timer_cnt = 0;
				
				if(++err_timer_cnt < 10)
				{
					Set_Color_Handle(255,255,0,0);
				}
				else if(err_timer_cnt < 20)
				{
					Set_Color_Handle(0,0,0,0);
				}
				else
				{
					err_timer_cnt = 0;
				}      
			}
			else
			{
				Multicolor_Lamp(RGB_Handle.Colour_Type); 
			}
		}
		else
		{
			if(Sys_Info.Chargestop_Flag)
			Set_Color_Handle(255,0,0,0);
		}
    
    Display_Batter_Usering(Batt_State.BatteryLevel);
  }
  else if(Sys_Info.Power_State == MACHINEOFF) //关机状态
  {
    Set_Color_Handle(0,0,0,0);
    Set_Color_Handle(0,0,0,1);
    Set_Color_Handle(0,0,0,2);
    if(Sys_Info.Charge_State == No_CHARGE_STA)  //非充电状态
    {
      LED_ALL_OFF;
      BACKLIGHT_OFF;
      Sys_Info.BKlight_State = Light_Off;  //红灯状态
    }
    else    //充电中
    {
      Display_Batter_Charge(Batt_State.BatteryLevel);
    }
  }
  else  //电量查看状态
  {
    if(Sys_Info.Charge_State == No_CHARGE_STA)  //非充电状态
    {
      if(++time_cnt <= 15)
      {
        Display_Batter_Level(Batt_State.BatteryLevel);  //电量查看
      }
      else
      {
        time_cnt = 0;
        LED_ALL_OFF;
        Sys_Info.Power_State = MACHINEOFF;
      }
    }
    else  //充电状态
    {
      Sys_Info.Power_State = MACHINEOFF;
    }
  }

  LCD_output_transf(Led_Colour);    //RGB灯刷新
}
/*************************************************************************************
* FunctionName	: LED_Display_Error  LED燈服務函數，100ms刷新一次
* Description   :
* EntryParameter:
* ReturnValue   :
**************************************************************************************/
void LED_Display_Error(void)
{
  static uint8_t time_cnt = 0;
  static uint16_t auto_off_cnt = 0;
  
  time_cnt++;
  auto_off_cnt++;
  
  if(auto_off_cnt > 165)
  {
    HAL_NVIC_SystemReset();
  }
  
  if(time_cnt < 10)
  {
    Set_Color_Handle(200,0,0,0);
    Set_Color_Handle(200,0,0,1);  
    Set_Color_Handle(200,0,0,2);
    LED1_ON;LED2_ON;LED3_ON;
  }
  else if(time_cnt < 15)
  {
    Set_Color_Handle(0,0,0,0);
    Set_Color_Handle(0,0,0,1);  
    Set_Color_Handle(0,0,0,2);
    LED1_OFF;LED2_OFF;LED3_OFF;
  }
  else if(time_cnt < 20)
  {
    LED1_OFF;LED2_OFF;LED3_OFF;
    Set_Color_Handle(200,0,0,0);
    
    if(Sys_Info.Err_State.bit.gyro_error_flag == SET ) //陀螺仪总线错误
    {
      LED1_ON;
    }
    if(Sys_Info.Err_State.bit.rgb_error_flag == SET ) //RGB总线错误
    {
      LED2_ON;
    }
    if(Sys_Info.Err_State.bit.head_ntc_error_flag == SET ) //治疗头NTC错误
    {
      LED3_ON;
    }
  }
  else if(time_cnt < 25)
  {
    LED1_OFF;LED2_OFF;LED3_OFF;
    Set_Color_Handle(200,0,0,1);
    if(Sys_Info.Err_State.bit.batt_ntc_error_flag == SET ) //电池NTC错误
    {
      LED1_ON;
    }
    if(Sys_Info.Err_State.bit.batt_value_error_flag == SET ) //电池电压错误
    {
      LED2_ON;
    }
    if(Sys_Info.Err_State.bit.usb_value_error_flag == SET ) //USB电压错误
    {
      LED3_ON;
    }
  }
  else if(time_cnt < 30)
  {
    LED1_OFF;LED2_OFF;LED3_OFF;
    Set_Color_Handle(200,0,0,2);
    if(Sys_Info.Err_State.bit.head_short_error_falg == SET ) //治疗头短路错误
    {
      LED1_ON;
    }
    if(Sys_Info.Err_State.bit.ems_value_error_flag == SET ) //EMS电压错误
    {
      LED2_ON;
    }
    if(Sys_Info.Err_State.bit.rf_value_error_flag == SET ) //RF电压错误
    {
      LED3_ON;
    }
  }
  else if(time_cnt < 35)
  {
    Set_Color_Handle(0,0,0,0);
    Set_Color_Handle(0,0,0,1);  
    Set_Color_Handle(0,0,0,2);
    LED1_OFF;LED2_OFF;LED3_OFF;
  }
  else
  {
    time_cnt = 0;
  }
   
   LCD_output_transf(Led_Colour);    //RGB灯刷新
}



