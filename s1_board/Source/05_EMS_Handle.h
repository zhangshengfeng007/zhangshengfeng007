#ifndef __EMS_HANDLE_H
#define __EMS_HANDLE_H

typedef enum
{
  EMS_Seal_One   = 21000,
  EMS_Seal_Two   = 23000,
  EMS_Seal_Three = 26500,
  EMS_Slide_One  = 21000,
  EMS_Slide_Two  = 24000,
  EMS_Slide_Three= 27000,//27500
}_EMS_Value_Typedef;

typedef struct
{
  PID_IncTypeDef  EMS_PID;
  uint16_t EMS_True_Value;
  uint8_t EmsControlFlage;
}_EMS_Handler_Typedef;
#define EMS_Start_DelayTime 10  //

extern _EMS_Handler_Typedef  EMS_Handler;

extern void EMS_OUT_PWM_Start(void);
extern void EMS_OUT_PWM_Stop(void);
extern void EMS_SET_VALUE_PWM_Start(void);
extern void EMS_SET_VALUE_PWM_Stop(void);
extern void EMS_PID_Init(void);
extern void EMS_Value_PID_Handler(void);
extern void EMS_OUT_Start(void);
extern void EMS_OUT_Stop(void);
extern void Set_Ems_Tim_Psc(uint8_t freq);

#endif

