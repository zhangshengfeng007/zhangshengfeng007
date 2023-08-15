#ifndef __ZJ_YG6550_H
#define __ZJ_YG6550_H

#include "main.h"


typedef struct {
  int16_t  AXIS_X;
  int16_t  AXIS_Y;
  int16_t  AXIS_Z;
} AxesRaw_t;
#define EVAL_I2Cx_TIMEOUT_MAX  10
extern AxesRaw_t GU6500XYZdataACCEL,GU6500XYZdataGYRO;


extern void GY6550_INIT(void);
extern void GY6500_Read(void);
extern uint8_t AttitudeProcessing(void);


#endif
