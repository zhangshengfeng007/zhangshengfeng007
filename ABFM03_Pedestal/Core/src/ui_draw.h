
#ifndef UI_DRAW_H
#define UI_DRAW_H

#include <stdint.h>
#include <stdbool.h>
#include "User_System_Config.h"

extern unsigned char lcd_framebuff[384];


typedef enum
{
	BAT_LOW_ALARM = 1,
	HEAT_OVER_ALARM,
	NTC_FAULT_ALARM,

}eALARM_TYPE;



//=================================================================


void OLED_SH1106_Init(void); //LCD��ʼ��
void ui_lcd_clear(uint8_t color);
void ui_draw_clear_screen(void); //����

void display_ems_level(uint8_t level_value);

void display_lock_and_radio_state(bool radio_state, bool lock_state); //��ʾ��������״̬ͼ�꣬������/����ͼ��
void display_wei_pei_dui(void);
void display_err_show(eALARM_TYPE err_value); //��ʾ�͵�/����/�¶�sensor������ʾ��

void display_full_screen_disconnect(void); //ȫ����ʾ�������ӶϿ�ͼ��

void update_to_oled(void); //LCDˢ����ʾ

void display_clear_lock(void);

void radio_state_display(_RADIO_STATE_e radio_state);
void lock_state_display(_LOCK_STATE lock_state);

#endif /* UI_DRAW_H */

