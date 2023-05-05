#ifndef __DRV_EMS_H
#define __DRV_EMS_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


//T9处电压 1.6v   ---// 约为    1.6*4096/3.3 =   1985

#define T9_0V05_ADC_VAL  57
#define T9_0V15_ADC_VAL  186   //   0.15 * 4096 / 3.3 =   186

#define T9_0V20_ADC_VAL  248
#define T9_0V25_ADC_VAL  310

#define T9_0V30_ADC_VAL  372
#define T9_0V40_ADC_VAL  496
#define T9_0V50_ADC_VAL  600
#define T9_0V70_ADC_VAL  848

#define T9_2V0_ADC_VAL   2482

#define T9_2V1_ADC_VAL   2606
#define T9_2V5_ADC_VAL   3103


/* Exported macro ------------------------------------------------------------*/

typedef struct{
	Func_Type State;	//状态
}Ems_Type;

typedef struct
{
	u16  prescaler;   // 鍒嗛?戠郴鏁?
	u16  period;      // 鎬荤殑璁℃暟鍊?
    u16  pulse;       // 鍛ㄦ湡 50%鏃跺�欑殑锛屾暟鍊煎彉鍖栫殑璁℃暟鍊?
}pwm_freq_config_t;


extern  Ems_Type Ems;

/* Exported functions ------------------------------------------------------- */
void Ems_Init(void);
void Ems_Control(Func_Type cmd);
void Ems_PWM_OFF(void);
void Ems_PWM_Config(void);
void EMS_Load_Detection(void);

#endif

