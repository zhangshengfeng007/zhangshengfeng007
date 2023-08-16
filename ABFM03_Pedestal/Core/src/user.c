#include "user.h"
#include "ui_draw.h"

void Lock_Check(void);
void Check_Master_State(void);
void User_OLED_Display(void);
void Charge_Process(void);


/**************************************/
// 功能：
//  1. 检测底座 无按键操作
//  2. 并且主机上无按键操作
//  3. 30s后，加锁
// 100ms 检测一次
/**************************************/
void Lock_Check(void)
{
    if(0 == SysInfo.Connect)
    {
        if(Locked == SysInfo.Lock_State)
        {
            SysInfo.Lock_State = Un_Lock;
			data_disp.Lock_State = Un_Lock;
            Auto_30S_Lock_Cnt = 0;
//            printf ("\n\r  sys_Open_clock \n\r");
            return;
        }
    }

    if(SysInfo.Lock_State == Un_Lock)  //
    {
        Auto_30S_Lock_Cnt ++;
        if(Auto_30S_Lock_Cnt >= 300)    //300
        {
//             printf ("\n\r  sys_Lock_clock \n\r");
            SysInfo.Lock_State = Locked;
            data_disp.Lock_State = Locked;
        }
    }

}

/**************************************/
//   显示刷新
//   1. 未连接时，显示 大图标 未连接
//   2. 连接时，如果未加锁，则显示： “档位”数字 + “强度”字样 + 连接图标 + 开锁图标
//   3. 连接时，如果加锁，  则显示： “档位”数字 + “强度”字样 + 连接图标 + 锁住图标
//   4. 连接时，如果加锁 +按键动作，  则显示： “档位”数字 + “强度”字样 + 连接图标 + 锁住图标（消失0.5s）后，回归第3种显示

/**************************************/
void connect_station_Display(void)
{
    if(0 == SysInfo.Connect)  // 没有连接
    {
        ui_draw_clear_screen();
	    display_full_screen_disconnect();
        return;
    }

    radio_state_display(CONNECTED);
    if(SysInfo.Lock_State == Un_Lock)  // 没 加锁时
    {
        lock_state_display(Un_Lock);
    }
	else if (data_disp.lock_undisp_flag)
    {
		display_clear_lock();
    }
    else
	{
        lock_state_display(Locked);
	}
}





void User_OLED_Display(void)
{
    // if(IDLE_120S_Cnt >= 120)
    // {
    //     OLED_Display_Off();  //锟斤拷锟斤拷
    // }
    // else
    // {
    //     if(Sys_Time == 80)
    //     {
    //         OLED_Display();	  //锟斤拷锟斤拷刷锟斤拷
    //     }
    // }
}

void Check_Master_State(void)
{
    // if((CHARGE_IO == 0) && (SysInfo.Master_State == Not_Online))
    // {
    //     if(CHECK_IN == Bit_RESET)
    //     {
    //         CHARGE_ON;
    //         CHARGE_IO = 1;
    //         SysInfo.Master_State = On_Line;
    //     }
    //     else
    //     {
    //         CHARGE_OFF;
    //         CHARGE_IO = 0;
    //         SysInfo.Master_State = Not_Online;
    //     }
    // }
}

void Charge_Process(void)
{
    // if(SysInfo.Master_State == On_Line)
    // {
    //     if(CHARGE_IO == 1)
    //     {
    //         CHARGE_ON;
    //         OP_ADC = ADC_GetData(ADC_CH_10_PB2);
    //         OP_ADC = OP_ADC * 3000 / 4095;

    //         if(Charge_Time >= 3000)
    //         {
    //             if(OP_ADC >= 2450)
    //             {
    //                 //CHARGE_OFF;
    //                 LED_G_OFF;
    //                 LED_W_OFF;
    //                 SysInfo.Err_State = Sys_Err;
    //             }
    //             else if((OP_ADC < 2450) && (OP_ADC > 100))
    //             {
    //                 CHARGE_ON;
    //                 LED_G_OFF;

    //                 if((LED_Time >= 1100) && (LED_Time < 2200))
    //                 {
    //                     TIM8->CCDAT4 = LED_Time - 1100;
    //                 }
    //                 else if(LED_Time > 2200)
    //                 {
    //                     LED_Time = 0;
    //                 }
    //                 else
    //                 {
    //                     TIM8->CCDAT4 = 1100 - LED_Time;
    //                 }

    //                 //锟斤拷锟斤拷锟斤拷锟?
    //             }
    //             else
    //             {
    //                 //锟斤拷锟斤拷
    //                 CHARGE_OFF;
    //                 CHARGE_IO = 0;
    //                 LED_G_ON;
    //                 LED_W_OFF;
    //                 Delay_MS(10);
    //             }
    //         }
    //         else
    //         {
    //             Charge_Time++;
    //         }
    //     }
    //     else
    //     {
    //         if(CHECK_IN != Bit_RESET)
    //         {
    //             Charge_Time = 0;
    //             SysInfo.Master_State = Not_Online;
    //         }
    //     }
    // }
    // else
    // {
    //     LED_G_OFF;
    //     LED_W_ON;
    // }
}
