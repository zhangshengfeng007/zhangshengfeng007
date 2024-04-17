#include "error_det.h"

xErrorStatus_t sx_ErrorStatus = {0};

void ErrorType_Reset(uint8_t errortype)
{
//	if(errortype&ERROR_TYPE_FAN)//���ȱ���
//		LED_MODE1_OFF();
//	if(errortype&ERROR_TYPE_VOLTAGE)//��ѹ����
//		LED_MODE2_OFF();
//	if(errortype&ERROR_TYPE_NTC)//�¶ȱ���
//		LED_MODE3_OFF();
	sx_ErrorStatus.b_errortype &= (~errortype);
}
uint8_t errordisplay_buf = 0;
void Error_Det_Handler(void)
{
	uint8_t errordisplay_flag = 0;
	
	if(sx_ErrorStatus.b_errortype)
	{
		if(++sx_ErrorStatus.uc_errorflashcnt==(200/20)){
			sx_ErrorStatus.b_errorflashdir=1;
			errordisplay_flag = 1;
		}
		if(sx_ErrorStatus.uc_errorflashcnt==(400/20)){
			sx_ErrorStatus.uc_errorflashcnt=0;
			sx_ErrorStatus.b_errorflashdir=0;
			errordisplay_flag = 1;
		}
		errordisplay_buf = 0x00;
		if(sx_ErrorStatus.b_errorflashdir){
//			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_SUPPLY)//���籨��
//				errordisplay_buf |= LED_MODE_5;
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_FAN)//���ȱ���
				errordisplay_buf |= LED_MODE_1 | LED_MODE_2;
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_VOLTAGE)//��ѹ����
				errordisplay_buf |= LED_MODE_1 | LED_MODE_2 | LED_MODE_3;
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_NTC)//�¶ȱ���
				errordisplay_buf |= LED_MODE_1 | LED_MODE_2 | LED_MODE_3 | LED_MODE_4;
		}else{
//			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_SUPPLY)//���籨��
//				errordisplay_buf &= ~LED_MODE_5;
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_FAN)//���ȱ���
				errordisplay_buf &= ~(LED_MODE_1 | LED_MODE_2);
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_VOLTAGE)//��ѹ����
				errordisplay_buf &= ~(LED_MODE_1 | LED_MODE_2 | LED_MODE_3);
			if(sx_ErrorStatus.b_errortype&ERROR_TYPE_NTC)//�¶ȱ���
				errordisplay_buf &= ~(LED_MODE_1 | LED_MODE_2 | LED_MODE_3 | LED_MODE_4);
		}
//		if(errordisplay_flag){
//			set_led_event(LED_EVENT_ERROR);
//			//SMG_Write_Byte(errordisplay_buf);
//		}
	}
}










