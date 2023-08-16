#ifndef __EMS_H
#define __EMS_H
#include "includes.h"

#define EMS_BOOST_ADC_AN	    0
#define EMS_VOL_ERROR			20
#define EMS_BOOST_DEADLINE		38
#define EMS_BOOST_SW_PERIOD		20		//20KHz  400
#define EMS_Mode                0
#define EMS_1000_ms             100
#define EMS_1500_ms             150
#define EMS_2000_ms             200
#define PSC_Constant            4
#define Frequency_PSC           0
//******100 -100 *****************//
#define MaxValue                50
#define MinValue                (-50)

#if ARF001
	#define EMS_VOL_Constant     85 // 85 1.5 V  114 2 V
	#define PidConstant          30
	#define	EMS_VOL_LEVEL1		(1228 + PidConstant)//21.569 V
	#define	EMS_VOL_LEVEL2		(1392 + PidConstant)//24.540 V
	#define	EMS_VOL_LEVEL3		(1556 + PidConstant)//27.331 V
	#define	EMS_VOL_LEVEL4		(1720 + PidConstant)//30.211 V
	#define	EMS_VOL_LEVEL5		(1884 + PidConstant)//33.092 V
#else
	#define PidConstant       	10
	#define	EMS_VOL_LEVEL1		(1228 + PidConstant)//1228 21.5V
	#define	EMS_VOL_LEVEL2		(1326 + PidConstant)//1351
	#define	EMS_VOL_LEVEL3		(1444 + PidConstant)//1474
	#define	EMS_VOL_LEVEL4		(1560 + PidConstant)//1597
	#define	EMS_VOL_LEVEL5		(1700  + PidConstant)//1720
#endif




#define EMS_PWR_EN_ON()   HAL_GPIO_WritePin(EMS_PWR_EN_PORT, EMS_PWR_EN_Pin, GPIO_PIN_SET)
#define EMS_PWR_EN_OFF()  HAL_GPIO_WritePin(EMS_PWR_EN_PORT, EMS_PWR_EN_Pin, GPIO_PIN_RESET)

typedef enum
{
  EMS_1KHZ_FREQ = 0,
  EMS_2KHZ_FREQ,
  EMS_3KHZ_FREQ,
  EMS_4KHZ_FREQ,
  EMS_ALL_FREQ,  // 测试模式，分为特定频率和所有频率
}EMS_FREQ_e;



/**************/
// 指针函数
//
typedef void (*dac_write)(uint16_t);
typedef void (*dac_read)(uint8_t);
typedef void (*dac_init)(void);

typedef struct
{
	dac_init p_init;
	dac_write p_write;
	dac_read  p_read;
	/* data */
}DAC_PROBE_t;



typedef struct
{
	uint8_t iic_addr;
	DAC_PROBE_t dac_probe;

}DAC_DEVICE_t;

extern DAC_DEVICE_t EMS_dac_dev;

void DAC_MCP4726_OUT(uint16_t Voltage);
void DAC_MCP4726_Init(void);
void DAC_SGM5355_OUT(uint16_t Voltage);
uint16_t DAC_SGM5355_read_reg(uint8_t reg);



void Ems_Osc_Off(void);
void Ems_Osc_On(void);
void Ems_Boost_Off(void);
void Ems_Boost_On(void );
uint16_t acquire_adc_value(uint8_t channel, uint16_t* data);
void Set_Ems_Value(_EMS_HANDLE_TypeDef* EmsLevel,uint8_t StepValue);
void Ems_Value_Add(_EMS_HANDLE_TypeDef* EmsLevel);
void Ems_Value_Reduce(_EMS_HANDLE_TypeDef* EmsLevel);
void upkeep_RF_Level(_EMS_HANDLE_TypeDef* EmsLevel);
void Ems_Osc_Process(void);
// void Ems_Boost_Process(void);
void EMS_Open_Process(void);
void Touch_Skin_EMS_Freq_Conversion(void);
void EMS_Pwm_Stop(void);
void EMS_Pwm_Start(void);
void Ems_DeInit(void);
void Ems_Init(void);
void EMS_Frequency_conversion(uint8_t PSC);
void Change_EMS_Freq(void);
void Ems_dac_probe(void);

#endif


