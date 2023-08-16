
#include "includes.h"
#include "Led_Display.h"

_LED_VALUE_TypeDef Led_Value;
_IRLED_VALUE_TypeDef IRled_Value;

uint8_t Led_Colour[15] = {0};//G R B
uint8_t LCD_data[120];



/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void RGB_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	static uint8_t Last_LCD_Data[121] = {0};
	static uint8_t Flag, Counts;

	Flag = memcmp(Last_LCD_Data, LCD_data, Size);

	if(Flag)// 显示有变化了再刷新
	{
		memcpy(Last_LCD_Data, LCD_data,Size);
		HAL_SPI_Transmit_DMA(hspi, LCD_data, sizeof(LCD_data));
	}
}
/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void LCD_Refresh(void)
{
  uint8_t i, j;
  uint8_t temp;

  for(i = 0; i < 15; i++)
  {
	temp = Led_Colour[i];

    for(j = 0; j < 8; j ++)
    {
      if((temp & 0x80) == 0x80)
      {
        LCD_data[8 * i + j] = CodeOne;
      }
      else
      {
        LCD_data[8 * i + j] = CodeZero;
      }
      temp <<= 1;
    }
  }
	//  LCD_data[120] = 0;
  RGB_SPI_Transmit(&hspi1, LCD_data, sizeof(LCD_data));
}

/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void RGB_Init(void)
{
  uint8_t i;

  for(i = 0 ; i < sizeof(Led_Colour); i ++)
  {
    Led_Colour[i] = 0x00;
  }
  LCD_Refresh();
}

/*************************************************************************************
* FunctionName	:
* Description	:
* EntryParameter:
* ReturnValue	:
**************************************************************************************/
void Set_Color_Handle(uint8_t R,uint8_t G,uint8_t B,uint8_t num)
{
  Led_Colour[num * 3 + 0] = G;
  Led_Colour[num * 3 + 1] = R;
  Led_Colour[num * 3 + 2] = B;
}
/**********************************************************
* FunctionName   :
* ReturnValue    : 当前需要显示的led设置成黑色
*******************************************************/
static void set_black_color(uint8_t led_index)
{
	Set_Color_Handle(Close_R, Close_G, Close_G, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :当前需要显示的led设置成白色
*******************************************************/
static void set_white_color(uint8_t led_index)
{
	Set_Color_Handle(White_R, White_R, White_R, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    : 当前需要显示的led设置成红色
*******************************************************/
static void set_red_color(uint8_t led_index)
{
	Set_Color_Handle(White_R, Close_G, Close_G, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    : 当前需要显示的led设置成绿色
*******************************************************/
static void set_green_color(uint8_t led_index)
{
	Set_Color_Handle(Close_G, White_R, Close_G, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :  当前需要显示的led设置成蓝色
*******************************************************/
static void set_blue_color(uint8_t led_index)
{
	Set_Color_Handle(Close_G, Close_G, White_R, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :当前需要显示的led设置成橘色
*******************************************************/
static void set_orange_color(uint8_t led_index)
{
	Set_Color_Handle(255, 97, 0, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :当前需要显示的led设置成紫色
*******************************************************/
static void set_purple_color(uint8_t led_index)
{
	Set_Color_Handle(128, 0, 128, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :当前需要显示的led设置成粉色
*******************************************************/
static void set_pink_color(uint8_t led_index)
{
	Set_Color_Handle(252, 105, 184, led_index - 1);
}

/**********************************************************
* FunctionName   :
* ReturnValue    :当前需要显示的led设置成淡青色
*******************************************************/
static void set_cyan_color(uint8_t led_index)
{
	Set_Color_Handle(64, 224, 208, led_index - 1);
}


/**************************************************************************************
* FunctionName   : led_scan(uint8_t bit_flag)
* Description    :
* EntryParmeter :
* ReturnValue    : None
**************************************************************************************/
void Level_led_scan(uint8_t cur_level)
{
	switch(cur_level)
	{
		case LEVEL1_DISP:
		{
			set_white_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
			break;
		}
		case LEVEL2_DISP:
		{
			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
			break;
		}

		case LEVEL3_DISP:
		{
			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
			break;
		}
		default:
		{
			set_black_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : led_scan(uint8_t bit_flag)
* Description    :
* EntryParmeter :
* ReturnValue    : None
**************************************************************************************/
void Level_leds_set(uint8_t State)
{
	switch(State)
	{
		case BLACK:
		{
			set_black_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
			break;
		}
		case White:
		{
			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
			break; //��ɫ
		}

		case RED:
		{
			set_red_color(LEVEL1_DISP);
			set_red_color(LEVEL2_DISP);
			set_red_color(LEVEL3_DISP);
			break;
		}
		case GREEN:
		{
			set_green_color(LEVEL1_DISP);
			set_green_color(LEVEL2_DISP);
			set_green_color(LEVEL3_DISP);
			break;
		}
		case BLUE:
		{
			set_blue_color(LEVEL1_DISP);
			set_blue_color(LEVEL2_DISP);
			set_blue_color(LEVEL3_DISP);
			break;
		}

		default:
		{
			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : Mode_led_scan(uint8_t State)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Mode_led_scan(uint8_t State)
{
	switch(State)
	{
		case White:
		{
			set_white_color(Mode_LED_DISP);
			break;
		}

		case Cyan:
		{
			set_cyan_color(Mode_LED_DISP);
			break;
		}

		case RED:
		{
			set_red_color(Mode_LED_DISP);
			break;
		}
		case GREEN:
		{
			set_green_color(Mode_LED_DISP);
			break;
		}
		case BLUE:
		{
			set_blue_color(Mode_LED_DISP);
			break;
		}
		case BLACK:
		{
			set_black_color(Mode_LED_DISP);
			break;
		}
		default:
		{
			set_black_color(Mode_LED_DISP);
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : Mode_led_scan(uint8_t State)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void State_led_scan(uint8_t State)
{
	switch(State)
	{
		case White:
		{
			set_white_color(STATUS_LED_DISP);
			break;    //白色
		}
		case Orange:
		{
			set_orange_color(STATUS_LED_DISP);
		    break;    //橙色
		}

		case Purple:
		{
			set_purple_color(STATUS_LED_DISP);
			break;    //紫色
		}

		case LightPink:
		{
			set_pink_color(STATUS_LED_DISP);
		 	break;    //粉色
		}

		case Cyan:
		{
			set_cyan_color(STATUS_LED_DISP);
			break;	  //淡青色
		}

		case RED:
		{
			set_red_color(STATUS_LED_DISP);
			break;
		}
		case GREEN:
		{
			set_green_color(STATUS_LED_DISP);
			break;
		}
		case BLUE:
		{
			set_blue_color(STATUS_LED_DISP);
			break;
		}
		case BLACK:
		{
			set_black_color(STATUS_LED_DISP);
			break;
		}
		default:
		{
			set_black_color(STATUS_LED_DISP);
			break;
		}

	}
}
/**************************************************************************************
* FunctionName   : Cur_Level_Twink_Display(uint8_t level,uint16_t stayTime)
* Description    : ������ָʾ����˸
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Cur_Level_Twink_Display(uint8_t level,uint16_t stayTime)
{
	static uint8_t Cur_level_on_flag;
	static uint16_t inverse_cnt;

	if(Cur_level_on_flag)
	{
		if(inverse_cnt)
		{
			inverse_cnt --;
		}
		else
		{
			Cur_level_on_flag = 0;
			Level_led_scan(level - 1);
		}
	}
	else
	{
		inverse_cnt ++;
		if(inverse_cnt >= stayTime)
		{
			Cur_level_on_flag = 1;
			inverse_cnt = stayTime;
			Level_led_scan(level);
		}
	}
}
/**************************************************************************************
* FunctionName   : Twink1hz_Display(uint8_t BitFlag,uint16_t stayTime)
* Description    : �͵絵λ�ƺ���
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Twink1hz_Display(uint8_t disp_level, uint16_t stayTime, uint8_t state_led_inverse_flag)
{
	static uint8_t Twink1hzState;
	static uint16_t Twink1hzcnt;

	if(Twink1hzState)
	{
		Twink1hzcnt--;
		if(Twink1hzcnt == 0)
		{
			Twink1hzState = 0;
			Twink1hzcnt = 0;
			Level_led_scan(LEVEL_NULL_DISP);
			if(state_led_inverse_flag)
			{
				State_led_scan(BLACK);
			}
		}
	}
	else
	{
		Twink1hzcnt++;
		if(Twink1hzcnt >= stayTime)
		{
			Twink1hzState = 1;
			Twink1hzcnt = stayTime;
			Level_led_scan(disp_level);
			if(state_led_inverse_flag)
			{
				State_led_scan(Orange); //����״̬������ɫ
			}
		}
	}

}
/**************************************************************************************
* FunctionName   : LedStay_Display(uint8_t BitFlag,uint16_t stayTime)
* Description    : ��ʱϨ��ָʾ��
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
// void LedStay_Display(uint8_t BitFlag,uint16_t stayTime)
// {
// 	static uint16_t TimeCnt2,TargeTime;

// 	if(++TimeCnt2>TargeTime)
// 	{
// 		TimeCnt2=TargeTime;
// 		Level_led_scan(LEVEL_NULL_DISP);
// 	}
// 	else
// 	{
// 		TargeTime=stayTime;
// //		TimeCnt=1;
// 		Level_led_scan(BitFlag);
// 	}
// }
/**************************************************************************************
* FunctionName   : Bat_low_led_Filcker(uint16_t stayTime)
* Description    : ������ѯ��ȱ��1����λ����˸5��
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Bat_low_led_Filcker(uint16_t stayTime)
{
	static uint8_t state = 0;
	static uint8_t inversed_delay_Cnt;

	if(state)
	{
		inversed_delay_Cnt ++;
		if(inversed_delay_Cnt > stayTime)
		{
			state = 0;
			Level_led_scan(LEVEL_NULL_DISP);
		}
	}
	else
	{
		if(0 == inversed_delay_Cnt)
		{
			Level_led_scan(LEVEL1_DISP);
			state = 1;
		}
		else
		{
			inversed_delay_Cnt --;
		}
	}
}

/**************************************************************************************
* FunctionName   : Charge_Led_Display(uint8_t Level)
* Description    :
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Charge_Led_Display(uint8_t Level)
{
	Mode_led_scan(BLACK);
	State_led_scan(BLACK);

	if(BAT_100_100_STATUS == SysInfo.Batt_Value.State)
	{
		Level_led_scan(LEVEL3_DISP);
	}
	else
	{
		Cur_Level_Twink_Display(Level, 100);
	}
}

/**************************************************************************************
* FunctionName   : Agine_ems_rf_out_Led_Display()
* Description    :  ems 放电
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Inset_life_ems_rf_out_Led_Display(void)
{
	// static uint8_t Cur_level_on_flag;
	// static uint16_t inverse_cnt;

	// if(Cur_level_on_flag)
	// {
	// 	if(inverse_cnt)
	// 	{
	// 		inverse_cnt --;
	// 	}
	// 	else
	// 	{
	// 		Cur_level_on_flag = 0;

	// 		set_black_color(LEVEL1_DISP);
	// 		set_white_color(LEVEL2_DISP);
	// 		set_white_color(LEVEL3_DISP);
	// 	}
	// }
	// else
	// {
	// 	inverse_cnt ++;
	// 	if(inverse_cnt >= 100)
	// 	{
	// 		Cur_level_on_flag = 1;
	// 		inverse_cnt = 100;
	// 		set_black_color(LEVEL1_DISP);
	// 		set_white_color(LEVEL2_DISP);
	// 		set_black_color(LEVEL3_DISP);
	// 	}
	// }

	set_white_color(LEVEL1_DISP);
	set_white_color(LEVEL2_DISP);
	set_white_color(LEVEL3_DISP);
}



/**************************************************************************************
* FunctionName   : Inset_life_charge_Led_Display()
* Description    :  寿命测试 --充电状态led显示
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Inset_life_charge_Led_Display(void)
{
	static uint8_t Cur_level_on_flag;
	static uint16_t inverse_cnt;

	if(Cur_level_on_flag)
	{
		if(inverse_cnt)
		{
			inverse_cnt --;
		}
		else
		{
			Cur_level_on_flag = 0;

			set_black_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
		}
	}
	else
	{
		inverse_cnt ++;
		if(inverse_cnt >= 100)
		{
			Cur_level_on_flag = 1;
			inverse_cnt = 100;
			set_black_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
		}
	}

}


/**************************************************************************************
* FunctionName   : Agine_ems_rf_out_Led_Display()
* Description    :  ems 放电
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Agine_ems_rf_out_Led_Display(void)
{
	static uint8_t Cur_level_on_flag;
	static uint16_t inverse_cnt;

	if(Cur_level_on_flag)
	{
		if(inverse_cnt)
		{
			inverse_cnt --;
		}
		else
		{
			Cur_level_on_flag = 0;

			set_black_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
		}
	}
	else
	{
		inverse_cnt ++;
		if(inverse_cnt >= 100)
		{
			Cur_level_on_flag = 1;
			inverse_cnt = 100;
			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
		}
	}
}


/**************************************************************************************
* FunctionName   : Agine_charge_Led_Display()
* Description    :  ems 放电
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Agine_charge_Led_Display(void)
{
	static uint8_t Cur_level_on_flag;
	static uint16_t inverse_cnt;

	if(Cur_level_on_flag)
	{
		if(inverse_cnt)
		{
			inverse_cnt --;
		}
		else
		{
			Cur_level_on_flag = 0;

			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
		}
	}
	else
	{
		inverse_cnt ++;
		if(inverse_cnt >= 100)
		{
			Cur_level_on_flag = 1;
			inverse_cnt = 100;
			set_white_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
		}
	}

}


/**************************************************************************************
* FunctionName   : Agine_finish_Led_Display()
* Description    :  ems 放电
* EntryParameter :
* ReturnValue    : None  10ms 进入一次
**************************************************************************************/
void Agine_finish_Led_Display(void)
{
	static uint8_t Cur_level_on_flag;
	static uint16_t inverse_cnt;

	if(Cur_level_on_flag)
	{
		if(inverse_cnt)
		{
			inverse_cnt --;
		}
		else
		{
			Cur_level_on_flag = 0;

			set_white_color(LEVEL1_DISP);
			set_white_color(LEVEL2_DISP);
			set_white_color(LEVEL3_DISP);
			set_cyan_color(Mode_LED_DISP);
			set_white_color(STATUS_LED_DISP);
		}
	}
	else
	{
		inverse_cnt ++;
		if(inverse_cnt >= 100)
		{
			Cur_level_on_flag = 1;
			inverse_cnt = 100;
			set_black_color(LEVEL1_DISP);
			set_black_color(LEVEL2_DISP);
			set_black_color(LEVEL3_DISP);
			set_black_color(Mode_LED_DISP);
			set_black_color(STATUS_LED_DISP);
		}
	}

}




/**************************************************************************************
* FunctionName   : Fault_Led_Display(uint8_t Fault)
* Description    :
* EntryParameter :
* ReturnValue    : None  10ms����һ��
**************************************************************************************/
void Fault_Led_Display(uint8_t Fault)
{
	switch(Fault)
	{
		case 0x00: break; //BAT_00_00_STATUS
		case 0x01: Twink1hz_Display(E1_Bat_Low,50,0);break;  //1��
		case 0x02: Twink1hz_Display(E1_Bat_Low,50,0);break;  //2��
		case 0x03: Twink1hz_Display(E1_Bat_Low,50,0);break;  //3��

		case 0x04:
		{
			Twink1hz_Display(E2_Temp_HIGHER_45, 50, 1);
			break;  //RF_NTC > 45°C
		}

		case 0x05:
		{
			Twink1hz_Display(E3_NTC_ERROR, 50, 1);
			break;  //NTC _ERR
		}
		case 0x06:
		{
		 	Twink1hz_Display(E1_Bat_Low, 50, 1);
		  break;  //
		}

		default :
		{

		}
		 break; //����ʾ
	}
}

/**************************************************************************************
* FunctionName   : state_led_disp(uint8_t Fault)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
static void state_led_disp(void)
{
	//if(!SysInfo.System_Err_status && SysInfo.MotionStateFlage)
	if(0 == SysInfo.Skin_Touch_Flag)
	{
		State_led_scan(BLACK); //离开皮肤，状态灯熄灭
		return;
	}

	if(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)
	{
		State_led_scan(BLACK); //过电流保护，状态灯熄灭
		return;
	}

	if(SysInfo.MotionStateFlage)
	{
		State_led_scan(White); //接触到皮肤，并且g_sensor 移动led亮起
	}
	else
	{
		State_led_scan(BLACK); // 2023 07 28 , 当接触皮肤，但是gensor无移动的时候，led熄灭，无能量输出
	}
}

/**************************************************************************************
* FunctionName   : Normal_Led_Display(uint8_t Fault)
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void Normal_Led_Display(uint8_t Level)
{
	Level_led_scan(Level);
}
/**************************************************************************************
* FunctionName   : sys_err_display(void)
* ReturnValue    : None
**************************************************************************************/
static void sys_err_display(void)
{
	switch(SysInfo.System_Err_status)
	{
		case System_LowPower_Fault:
		{
			Twink1hz_Display(E1_Bat_Low, 50, 1);
			break;
		}
		case System_OverTemp_Fault:
		{
			Twink1hz_Display(E2_Temp_HIGHER_45, 50, 1);
			break;
		}
		case System_NTC_Fault:
		{
			Twink1hz_Display(E3_NTC_ERROR, 50, 1);
			break;
		}

		default:
		{
			break;
		}
	}
}

/**************************************************************************************
* FunctionName   : test_mode_led_disp(void)
* ReturnValue    : None
**************************************************************************************/

static uint8_t Led_test_mode_Display(void)
{
	static uint8_t led_inverse_cnt = 0;
	static uint8_t cur_led_color = RED;


	led_inverse_cnt ++;
	if(led_inverse_cnt < 50)
	{
		return 0;
	}
	led_inverse_cnt = 0;

	State_led_scan(cur_led_color);
	Mode_led_scan(cur_led_color);
	Level_leds_set(cur_led_color);

	switch (cur_led_color)
	{
		case White:
		{
			cur_led_color = Orange;
			IRled_start();
			break;
		}

		case Orange:
		{
			cur_led_color = Purple;
			IRled_stop();
			break;
		}

		case Purple:
		{
			cur_led_color = LightPink;
			IRled_start();
			break;
		}
		case LightPink:
		{
			cur_led_color = Cyan;
			IRled_stop();
			break;
		}
		case Cyan:
		{
			cur_led_color = White;
			IRled_start();
			break;
		}

		case RED:
		{
			cur_led_color = GREEN;
			IRled_stop();
			break;
		}

		case GREEN:
		{
			cur_led_color = BLUE;
			IRled_start();
			break;
		}
		case BLUE:
		{
			cur_led_color = RED;
			IRled_start();
			break;
		}

		default:
		{
			cur_led_color = White;
			IRled_stop();
			break;
		}
	}

	return 1;
}

/**************************************************************************************
* FunctionName   : status_led_flicker(void)
* ReturnValue    : None
**************************************************************************************/
static void status_led_flicker(void)
{
	static uint8_t  inverse_cnt, inverse_flag;

	inverse_cnt ++;
	if(inverse_cnt > 50)
	{
		inverse_cnt = 0;
		if(inverse_flag)
		{
			State_led_scan(White);
			inverse_flag = 0;
		}
		else
		{
			State_led_scan(BLACK);
			inverse_flag = 1;
		}
		Level_led_scan(LEVEL_NULL_DISP);
	}
}

/**************************************************************************************
* FunctionName   : status_led_flicker(void)
* ReturnValue    : None
**************************************************************************************/
static void Status_Mode_led_flicker(void)
{
	static uint8_t  inverse_cnt, inverse_flag;

	inverse_cnt ++;
	if(inverse_cnt > 50)
	{
		inverse_cnt = 0;
		if(inverse_flag)
		{
			set_white_color(STATUS_LED_DISP);
			set_red_color(Mode_LED_DISP);
			inverse_flag = 0;
		}
		else
		{
			set_black_color(STATUS_LED_DISP);
			set_black_color(Mode_LED_DISP);
			inverse_flag = 1;
		}
		Level_led_scan(LEVEL_NULL_DISP);
	}
}


/**************************************************************************************
* FunctionName   : status_led_flicker(void)
* ReturnValue    : None
**************************************************************************************/
static void Auto_match_2_4G_disp(void)
{
	Status_Mode_led_flicker();
}



/**************************************************************************************
* FunctionName   : test_mode_led_disp(void)
* ReturnValue    : None
**************************************************************************************/
static void test_mode_led_disp(void)
{
	static uint8_t  last_disp_mode;
	static uint8_t led_disp_color;
	// static uint8_t  inverse_cnt, inverse_flag;

	switch(SysInfo.Test_Mode.STATUS)
	{
		case NONE_TEST:
		{
			status_led_flicker();
			break;
		}

		case TILA_MODE_RF_TEST:
		{
			State_led_scan(Purple);
			Mode_led_scan(Cyan);
			Normal_Led_Display(SysInfo.Save_Data.upkeep_level);
			break;
		}
		case NTC_WITH_OUTPUT_TEST:
		case XIUFU_MODE_RF_TEST:
		{
			State_led_scan(Purple);
			Mode_led_scan(White);
			Normal_Led_Display(SysInfo.Save_Data.repair_level);
			break;
		}

		case TILA_MODE_EMS_TEST:
		{
			State_led_scan(LightPink);
			Mode_led_scan(Cyan);
			Normal_Led_Display(SysInfo.Save_Data.upkeep_level);
			break;
		}

		case WIRELESS_TEST:
		{
			State_led_scan(Cyan);
		    Mode_led_scan(Cyan);
			Normal_Led_Display(SysInfo.Save_Data.upkeep_level);
		    break;
		}
		case READ_NTC_TEST:
		{
			State_led_scan(Cyan);
			Mode_led_scan(BLACK);
			Level_leds_set(BLACK);
			break;
		}

		case KEY_TEST:
		{
			break;
		}

		case LED_TEST:
		{
			Led_test_mode_Display();
			break;
		}

		case Gsensor_TEST:
		{
			if(SysInfo.MotionStateFlage)
			{
				State_led_scan(White);
			}
			else
			{
				State_led_scan(BLACK);
			}
			break;
		}

		case AGING_TEST:
		{
			// if(SysInfo.Test_Mode.display_Charge_flag)
			// {
			// 	Agine_charge_Led_Display();
			// 	IRled_stop();
			// }
			// else if(SysInfo.Test_Mode.EMS_RF_out_flag)
			// {
			// 	Agine_ems_rf_out_Led_Display();
			// 	IRled_start();
			// }
			// else
			// {
			// 	Agine_finish_Led_Display();
			// 	IRled_stop();
			// }
			IRled_start();
			if(SysInfo.Test_Mode.Aging_run_once_over_flag)
			{
				Agine_finish_Led_Display();
			}
			else
			{
				Agine_ems_rf_out_Led_Display();
				Mode_led_scan(Cyan);
				State_led_scan(White);
			}
			break;
		}

		case INSET_LIFE_TEST:
		{
			// if(SysInfo.Test_Mode.display_Charge_flag)
			// {
			// 	Inset_life_charge_Led_Display();
			// 	IRled_stop();
			// }
			// else if(SysInfo.Test_Mode.EMS_RF_out_flag)
			// {
			// 	Inset_life_ems_rf_out_Led_Display();
			// 	IRled_start();
			// }
			Inset_life_ems_rf_out_Led_Display();
			IRled_start();
			Mode_led_scan(Cyan);
			State_led_scan(White);
			break;
		}

		case ADJUST_NTC_TEST:
		{
			Mode_led_scan(Purple);
			State_led_scan(Purple);
			break;
		}

		default:
		{
			break;
		}
	}

}


/**************************************************************************************
* FunctionName   : Led_Display(_LED_VALUE_TypeDef * LED)
* Description    :
* EntryParameter :
* ReturnValue    : None 10ms����һ��
**************************************************************************************/
uint8_t Led_Display(_LED_VALUE_TypeDef * LED )
{

	if(SysInfo.System_Err_status && (SysInfo.Power_Value.state == System_ON) && (LED->Mode) != Batt_Charging)
	{
		sys_err_display();
		LCD_Refresh();
		return 0;
	}

	switch(LED->Mode)
	{
		case Batt_Charging:
		{
			Charge_Led_Display(LED->Level);
			break;
		}
		case Batt_Low_20:
		{
			//Fault_Led_Display(LED->Level);
			//break;
		}
		case Batt_Low_0:
		{
			Bat_low_led_Filcker(50);
			break;
		}
		case Batt_Normal:
		{
			Normal_Led_Display(LED->Level);
			state_led_disp();
			if(SysInfo.WorkState != Standy_mode && ((SysInfo.Power_Value.state == System_ON)||(SysInfo.Power_Value.state == System_SHORT_CIRCUIT_PROTECT)))
			{
				if(repair_mode == SysInfo.WorkState)
				{
					Mode_led_scan(White);
				}
				else
				{
					Mode_led_scan(Cyan);
				}
			}
			break;
		}
		case WAKE_UP_DISP:
		{
			if(LED->Level)
			{
				Normal_Led_Display(LED->Level);
			}
			else
			{
				Twink1hz_Display(E1_Bat_Low, 50, 0);
			}
			break;
		}
		case SLEEP_MODE_DISP:
		{
			Normal_Led_Display(LEVEL_NULL_DISP);
			break;
		}
		case TEST_MODE_DISP:
		{
			test_mode_led_disp();
			break;
		}

		case AUTO_MATCH_2_4G:
		{
			Auto_match_2_4G_disp();
			break;
		}

		default:
		{
			break;
		}
	}

	if((System_OFF == SysInfo.Power_Value.state) && (OFF == SysInfo.Test_Mode.Test_Mode_Flag))
	{
		Mode_led_scan(BLACK);
		State_led_scan(BLACK);
		LED_IR_OFF();
	}

	LCD_Refresh();
	return 0;
}
/**************************************************************************************
* FunctionName   : IRled_stop
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void IRled_stop(void)
{
	LED_IR_OFF();
}

/**************************************************************************************
* FunctionName   : IRled_start
* Description    :
* EntryParameter :
* ReturnValue    : None
**************************************************************************************/
void IRled_start(void)
{
	LED_IR_ON();
	VBAT_OUT_ON() ;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


