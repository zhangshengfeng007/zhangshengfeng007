#ifndef __DRV_EMS_H
#define __DRV_EMS_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#define T9_0V5_ADC_VAL   600
#define T9_2V0_ADC_VAL   2482

#define T9_2V1_ADC_VAL   2606
#define T9_2V5_ADC_VAL   3103


/* Exported macro ------------------------------------------------------------*/

typedef struct{
	Func_Type State;	//״̬
}Ems_Type;

typedef struct
{
	u16  prescaler;   // 分�?�系�?
	u16  period;      // 总的计数�?
    u16  pulse;       // 周期 50%时候的，数值变化的计数�?
}pwm_freq_config_t;


extern  Ems_Type Ems;

/* Exported functions ------------------------------------------------------- */
void Ems_Init(void);
void Ems_Control(Func_Type cmd);
void Ems_PWM_OFF(void);
void Ems_PWM_Config(void);
void EMS_Load_Detection(void);

#endif

