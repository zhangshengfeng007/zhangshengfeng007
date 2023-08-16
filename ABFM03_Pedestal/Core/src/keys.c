#include "keys.h"
#include "uart.h"
void sys_key_check(void);

_Key_State_Struct    Add_Key_State,Dec_Key_State;
extern _TEST_MODE_HANDLE_TypeDef test_mode;

/******************************************/
//5ms �̡¨�?��?��?
/******************************************/
void Add_Key_Check(void)
{
	if(IS_ADD_KEY_PRESS())
	{
		 if((Add_Key_State.press_cnt == KEY_SHORT_TIME)&&(Add_Key_State.Key_State == State_Up))
		 {
			Add_Key_State.Key_State = State_Down;
			Add_Key_State.Key_down_State =  Short_Press;
		    if(SysInfo.Lock_State == Un_Lock)
			{
				SysInfo.Level_State = Add_State;
				if(Level_30 == SysInfo.Level_TypeDef) //��?�䨮?��
				{
					data_disp.level_inverse_cnt = MAX_INVESER_CNT;
				}

			}
			else
			{
				SysInfo.Lock_Level_State = Add_State;

				data_disp.lock_inverse_cnt = MAX_INVESER_CNT;
			}

			if(test_mode.Key_Flag==0x01)
			{
				test_mode.Key_State=0x01;
			}

			  Auto_30S_Lock_Cnt=0;
			  IDLE_120S_Cnt = 0;
		 }
		 else
		 {
			 if(Add_Key_State.press_cnt < KEY_SHORT_TIME)
			 Add_Key_State.press_cnt++;
		 }
		 set_normal_freq();
	}
	else
	{
		 Add_Key_State.press_cnt=0;
		 Add_Key_State.Key_State = State_Up;
	}
}

/******************************************/
//5ms �̡¨�?��?��?
/******************************************/

void Dec_Key_Check(void)
{
	if(IS_DEC_KEY_PRESS())
	{
		 if((Dec_Key_State.press_cnt == KEY_SHORT_TIME)&&(Dec_Key_State.Key_State == State_Up))
		 {
				Dec_Key_State.Key_State = State_Down;
				Dec_Key_State.Key_down_State =  Short_Press;
			    if(SysInfo.Lock_State == Un_Lock)
				{
					SysInfo.Level_State = Subtract_State;

					if(Level_1 == SysInfo.Level_TypeDef) //��?�䨮?��
					{
						data_disp.level_inverse_cnt = MAX_INVESER_CNT;
					}
				}
				else
				{
					SysInfo.Lock_Level_State = Subtract_State;

					data_disp.lock_inverse_cnt = MAX_INVESER_CNT;
				}

				if(test_mode.Key_Flag==0x01)
				{
					test_mode.Key_State=0x02;
				}

			 	Auto_30S_Lock_Cnt=0;
			  IDLE_120S_Cnt = 0;
		 }
		 else
		 {
			  if(Dec_Key_State.press_cnt < KEY_SHORT_TIME)
			  Dec_Key_State.press_cnt++;
		 }
		 //set_auto_math_freq_2470M();

	    //   if(hw_2_4G_data.mode == AUTO_MATCH_MODE)
	    //   {
	    //        hw_2_4G_data.mode = NORMAL_MODE;
		// 	   HW2000_Init_250k();
	    //   }
	    //   else
	    //   {
	    //     hw_2_4G_data.mode = AUTO_MATCH_MODE;
		// 	HW2000_Init_250k();
	    //   }
	}
	else
	{
		 Dec_Key_State.press_cnt=0;
		 Dec_Key_State.Key_State = State_Up;
	}
}

/**************************************************************************************
* FunctionName   : Key_2_4G_auto_match_mode_Check(void)
* Description    : ����ģʽ��������
* EntryParameter : None
* ReturnValue    : None    5ms 调用一�?
**************************************************************************************/
void Key_2_4G_auto_match_mode_Check(void)
{
	static uint16_t enter_auto_match_delay = 0;

	if(IS_DEC_KEY_PRESS() && IS_ADD_KEY_PRESS())
	{
		enter_auto_match_delay ++;
		if(enter_auto_match_delay > 400)
		{
		    hw_2_4G_data.mode = AUTO_MATCH_MODE;
			enter_auto_match_delay = 0;
			HW2000_Init_250k();
		}
	}
	else
	{
		enter_auto_match_delay = 0;
	}
}

/*5ms??��?��?��?���?��?��2a*/
void sys_key_check(void)
{
	Add_Key_Check();
	Dec_Key_Check();
}

