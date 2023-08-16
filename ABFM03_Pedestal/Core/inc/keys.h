#ifndef __KEYS_H
#define __KEYS_H

#include "User_System_Config.h"


typedef enum
{

	State_Unkown,
  State_Up,
  State_Down,

}Key_Input_state;

typedef enum
{

	  Unkown,
	  Long_Press,
	  Short_Press,

}Key_down_time;

typedef struct
{
    uint32_t        	press_cnt;
	  Key_down_time   	Key_down_State;
    Key_Input_state 	Key_State;

}_Key_State_Struct;



extern _Key_State_Struct    Add_Key_State,Dec_Key_State;

void sys_key_check(void);
#endif
