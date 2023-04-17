/***********************************************************
* @file    task_app.c
* @brief  �������
* @version V1.0
* @date    2023.02.02
***********************************************************/
#include "task_app.h"


void (* Task_Arr[TASK_MAX])(void) = {Listen_Task0,Out_Task1,Sys_Task2};  //������������
u16 Task_Time_Arr[TASK_MAX] = {0};  //��������ʱ���������


/*
*************************************************************
* ����˵��: �������񣬲����¼�����������ϵͳ������
* ��    ��: ��
* �� �� ֵ: ��
*           ������ 1ms ����һ��
*************************************************************
*/
void Listen_Task0(void)
{
	static u16 count = 0;

	if(count % KEY_SCAN_CYCLE == 0)
	{
		Key_Scan_Process();
		Sensor_Process();
	}

	if(count % 10 == 0)
	{
		Bat_Temp_Read();
		RF_Temp_Read();
		count = 0;
	}

	if((Device.State == SYS_ON)||(Device.State == SYS_TEST)||(SYS_ADJUST_RELOAD == Device.State))
	{
		//RF_Load_Detection();
		EMS_Load_Detection();
	}

	if((RF.State == Func_DISABLE) && (Ems.State == Func_DISABLE))
	{
		if(RF.duty_time + Device.EMS_Time + 4 < _OUT_TIME_)
		{
			Bat_Level_Detection();
		}
	}

	//����ָ���
	if(Uart_RX_Finish == Func_FINISH)
	{
		Test_Order_Process();
	}

	count++;
	Task_Time_Arr[0] = 1;	//������������MS
}

/*
*************************************************************
* ����˵��: �����������
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Out_Task1(void)
{
	static u16 count = 0;
	static u16 sys_adjust_cnt = 0;
	switch(Device.State)
	{
		case SYS_ON:
		{
			Device_Out_Process();
			break;
		}
		case SYS_TEST:
		{
			Test_Mode_Device_Out_Process();
			break;
		}
		case SYS_ADJUST_RELOAD:
		{
			sys_adjust_cnt ++;
			RF_Control(Func_DISABLE);
			switch (sys_adjust_cnt)
			{
				case 1:
				{
					Ems_Control(Func_ENABLE);
					break;
				}
				case 3:   //6
				{
					Ems_Control(Func_DISABLE);
					break;
				}
			}

			if (sys_adjust_cnt >= 25)
			{
				sys_adjust_cnt = 0;
			}
			break;
		}
		default:
		{

			break;
		}
	}

	// //��Դָʾ����ʾ
	count++;
	if(count % 10 == 0)
	{
		Power_Led_Process();
		count = 0;
	}

	Motor_Process_Cycle();
	Task_Time_Arr[1] = 1;	//������������MS
}

/*
*************************************************************
* ����˵��: ϵͳ����
* ��    ��: ��
* �� �� ֵ: �� 100ms ����һ��
*************************************************************
*/
void Sys_Task2(void)
{
	static u16 count = 0;
	static u8 Bat_tmp_err_cnt = 0;	//�����˲�
	static u8 err2 = 0;
	static u8 err3 = 0;
	u16 tmp_val;
	static u16 printf_delay = 0;

#if (_WDT_ENABLE)
	/* reload FWDGT counter */
	fwdgt_counter_reload();
#endif

	printf_delay ++;
	if(0 == printf_delay % 10)
	{
		
		//  printf("\r\n RF.Load :%d, Adc_val: %d, level_adc:%d\r\n", RF.Load, Debug_data.dbg_val , RF.UnLoad_ADC_Val);
		//   printf("\r\n RF.Load= %d, Adc_val = %d,\r\n", RF.Load, Debug_data.dbg_val);
		// printf("T2_val =%d mV,Bat_val = %d mv\r\n", Power.adc_val * 3300 / 4096, Power.adc_val * 3300 * 3 / 2 /4096);
		//    printf("\r\n adc :%d, BatLevel = %d\r\n",Power.adc_val, Power.BatLevel);
		   //printf("\r\n Test.Aging_finish_flag :%d, BatLevel = %d\r\n",Power.adc_val, Power.BatLevel);

		//printf("RF_tmp=%d.%d *C, Rf_time =%d\r\n", RF.Temp/10, RF.Temp%10, RF.duty_time);
	    // printf("Device.State = %d, Device.Err_State = %d\r\n", Device.State, Device.Err_State);
		//printf("Aging_charge_flag = %d, Aging_finish_flag = %d\r\n", Test.Aging_charge_flag, Test.Aging_finish_flag);
		// printf("Aging_ems_rf_flag = %d, Aging_ems_rf_cnt = %d\r\n", Test.Aging_ems_rf_flag, Test.Aging_ems_rf_cnt);
		// printf("Power.adc_val = %d \r\n", Power.adc_val);

		printf_delay =0;
	}

	//�Զ��ػ�
	if(count%10 == 0){
		Device_AutoOff_Process();
	}

	switch(Device.State)  //
	{
		case SYS_TEST:
		{
			Test_mode_Charge_Ctrl();
			break;
		}
		default:
		{
			//���
			Normal_mode_Charge_Ctrl();
			break;
		}
	}
	//����
#if	_BAT_TEMP_ERR_
//����¶ȱ�����
  /*
   * ����¶ȷ�Χ ��0~45��C
   * �ŵ��¶ȷ�Χ ��-10��C~60��C
   */
	bat_temp_err_check();
#endif

#if	_BAT_LEVEL_ERR_
	bat_val_err_check();
#endif

#if	_RF_TEMP_ERR_
	rf_temp_err_check();
#endif

	// �缫ͷNTCУ׼����
	if((Test.State == Func_ENABLE)&&(Test.mode_sta == Ntc_Set_STA))
	{
		if(count%10 == 0){
			RF_Ntc_Adjust_Process();
		}
	}
	count++;
	Task_Time_Arr[2] = 100;	//������������MS
}
