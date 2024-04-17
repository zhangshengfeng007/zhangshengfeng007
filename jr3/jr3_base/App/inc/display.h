#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "system.h"

typedef enum 
{
	LED_SENSE_ON,
	LED_SENSE_ROTARY,
	LED_SENSE_TOUCH,
	LED_SENSE_SLEEP
}led_sense_type;

typedef enum 
{
    LED_EVENT_NULL,
	LED_EVENT_MODE			=0x01,
    LED_EVENT_AUTO			=0x02,
	LED_EVENT_SENCE			=0x04,
	LED_EVENT_ERROR			=0x08,
	LED_EVENT_BURNIN		=0x10,
	LED_EVENT_CLEAR			=0x20,
    LED_EVENT_OFF			=0x80,
}led_event_type;

typedef struct xDisplayStatus_t_
{
	uint8_t b_Event;
	uint8_t b_sensetype;
	uint8_t b_senseonoffflag;
	uint8_t b_sensebreathjustdir;
	//uint8_t b_sensebreathcnt;
	uint16_t b_sensebreathduty;
//	uint8_t b_sensebreathchangecnt;
//	uint8_t b_sensebreathfre;
	uint16_t b_sensebreathjustcnt;
	uint16_t uc_senseonoffcnt;
	uint16_t sense_fre;
}xDisplayStatus_t;

extern xdata xDisplayStatus_t sx_DisplayStatus_t;
extern uint8_t LED_send_buf;

#define		set_led_event(x)				sx_DisplayStatus_t.b_Event|=x
#define		reset_led_event(x)				sx_DisplayStatus_t.b_Event&=~x
//#define		LED_SenseCnt_Reset()			{sx_DisplayStatus_t.uc_senseonoffcnt=0;sx_DisplayStatus_t.b_senseonoffflag=false;}
#define		LED_SensetypeFlag_Get()			sx_DisplayStatus_t.b_sensetype
#define		LED_ErrorBuff_Set(x)			sx_DisplayStatus_t.b_errorbuff=x
#define		LED_ErrorBuff_Get()				sx_DisplayStatus_t.b_errorbuff

//#define		LED_MODE1_ON()					{LED_send_buf&=~LED_MODE_1;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE2_ON()					{LED_send_buf&=~LED_MODE_2;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE3_ON()					{LED_send_buf&=~LED_MODE_3;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE4_ON()					{LED_send_buf&=~LED_MODE_4;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE5_ON()					{LED_send_buf&=~LED_MODE_5;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE1_OFF()					{LED_send_buf|=LED_MODE_1;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE2_OFF()					{LED_send_buf|=LED_MODE_2;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE3_OFF()					{LED_send_buf|=LED_MODE_3;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE4_OFF()					{LED_send_buf|=LED_MODE_4;SMG_Write_Byte(LED_send_buf);}
//#define		LED_MODE5_OFF()					{LED_send_buf|=LED_MODE_5;SMG_Write_Byte(LED_send_buf);}


void Display_init(void);
void Display_Cb(void);
void Display_Breath_Cb(void);
void Display_Handler(void);
void LED_SensetypeFlag_Set(uint8_t value);

#endif
