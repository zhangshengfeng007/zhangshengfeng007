#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32g0xx_hal.h"
#include "00_Handle.h"
#include "01_Key_Process.h"
#include "02_LTR831.h"
#include "03_YG6550.h"
#include "04_RF_Handle.h"
#include "05_EMS_Handle.h"
#include "06_BATT_Handle.h"
#include "07_MOTO_Handle.h"
#include "08_LED_Handle.h"
#include "09_OUTPUT.h"
#include "10_EEPROM.h"
#include "11_UART_Send.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//#define Test_Reset_Cause
#define IWDG_ENABLE
//#define CELL_TEST_MODE        //ϸ������ʵ��汾��ȡ���Զ��ػ������ᴫ��������
#define ENABLE_CURR_PROTECT
//#define ENABLE_CHECK_SYS_ERROR

//#define COLOR_SENSOR_ENABLE     // ���ʹ������ɫ������ 

#endif


