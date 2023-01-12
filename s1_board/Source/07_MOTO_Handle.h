#ifndef __MOTO_HANDLE_H
#define __MOTO_HANDLE_H

typedef enum
{
  None,
  SEAL_TIME_END,
}_MOTO_RUN_enum;

typedef enum
{
  Ready,
  Start,
  Runing,
  End,
}_MOTO_State_enum;

typedef struct
{
  _MOTO_RUN_enum Moto_Flag;
  _MOTO_RUN_enum Moto_Temp;
  _MOTO_RUN_enum Moto_RunMode;
  _MOTO_State_enum Moto_State;
  uint8_t Electric_Machinery_Onoff;///电机开关
  uint16_t Moto_Cnt;
}_Moto_Control_Typedef;

extern _Moto_Control_Typedef  Moto_Control;

extern void MOTO_Handle(void);
extern void Moto_Set_Funtion(void);
extern void buzzerChirp(uint8_t slide_mode,uint8_t mode, uint8_t Seal_gear,uint8_t Slide_gear,uint8_t onoff,uint8_t load,uint8_t color);

#endif

