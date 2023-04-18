#ifndef __DRV_MOTOR_H
#define __DRV_MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


/* Exported macro ------------------------------------------------------------*/

#define  MOTOR_ON		gpio_bit_write(MOTOR_PORT, MOTOR_PIN,SET)
#define  MOTOR_OFF 		gpio_bit_write(MOTOR_PORT, MOTOR_PIN,RESET)


typedef struct{
	Func_Type State;
	u16 OutTime;	//Æô¶¯Ê±¼ä		
}Motor_Type;


	
/* Exported functions ------------------------------------------------------- */
void Motor_Init(void);
void Motor_Start_Time(u16 Time);
void Motor_Stop(void);
void Motor_Process_Cycle(void);

#endif

