#ifndef _TASK_APP_H
#define _TASK_APP_H

#include "common.h"
#include "app_include.h"

#define TASK_MAX	3

extern void (* Task_Arr[TASK_MAX])(void);
extern u16 Task_Time_Arr[TASK_MAX];

void Listen_Task0(void);
void Out_Task1(void);
void Sys_Task2(void);

#endif




