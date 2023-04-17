#ifndef __DRV_EMS_RF_EN_PIN_H
#define __DRV_EMS_RF_EN_PIN_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/
typedef enum
{
    EN_PIN_EMS_MODE = 0,
    EN_PIN_RF_MODE,
    EN_PIN_CLOSE_MODE,
}en_pin_output_mode_e;

void timer15_init(void);
void ems_rf_en_change(en_pin_output_mode_e en_mode);
#endif

