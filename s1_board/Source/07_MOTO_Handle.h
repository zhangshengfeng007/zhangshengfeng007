#ifndef __MOTO_HANDLE_H
#define __MOTO_HANDLE_H

typedef enum
{
  None,
  SEAL_TIME_END,
}_motor_run_e;

typedef enum
{
  M_SHORT,
  M_LONG,
}_motor_type_e;


typedef enum
{
  Ready,
  Start,
  Runing,
  End,
}_motor_state_e;

typedef struct
{
  _motor_run_e Moto_Flag;
  _motor_run_e Moto_Temp;
  _motor_type_e type;
  _motor_run_e Moto_RunMode;
  _motor_state_e m_state;
  uint8_t Electric_Machinery_Onoff;///电机开关
  uint16_t m_run_Cnt;
}_motor_data_t;

extern _motor_data_t  motor_data;

extern void MOTO_Handle(void);
extern void Moto_Set_Funtion(void);
extern void buzzerChirp(uint8_t slide_mode,uint8_t mode, uint8_t Seal_gear,uint8_t Slide_gear,uint8_t onoff,uint8_t load,uint8_t color);

#endif

