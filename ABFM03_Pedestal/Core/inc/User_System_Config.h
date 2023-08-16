#ifndef __USER_SYSTEM_CONFIG_H
#define __USER_SYSTEM_CONFIG_H

#include "main.h"
#include "n32g030.h"
#include "n32g030_rcc.h"
#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
#include "SSD1317_OLED.h"
#include "HW2000.h"
#include "spi.h"
#include "exit.h"
#include "time_interrupt.h"
#include "user.h"
#include "adc.h"
#include "n32g030_tim.h"
#include "keys.h"

typedef enum
{
	 No_State,
	 Add_State,
   Subtract_State,

}_LEVEL_STATE;

typedef enum
{
	Attonity,
	Locked,     // µ××ùËø×¡ --30sÄÚÎÞ½ÓÊÕÊý¾Ý£¬¾ÍËø×¡
	Un_Lock,      //
}_LOCK_STATE;


typedef enum
{
	CONNECTED = 0,     // Á¬½ÓÐ¡Í¼±ê
	UN_CONNECT,      //
}_RADIO_STATE_e;



typedef enum
{
	Sys_Err_None,
	Low_Batter,
	Temp_High_Err,
	Temp_Fault_Err,
	Sys_Err,
}_ERR_STATE;
typedef enum
{
		Level_0,
		Level_1,
		Level_2,
		Level_3,
		Level_4,
		Level_5,
		Level_6,
		Level_7,
		Level_8,
		Level_9,
		Level_10,
		Level_11,
		Level_12,
		Level_13,
		Level_14,
		Level_15,
		Level_16,
		Level_17,
		Level_18,
		Level_19,
		Level_20,
		Level_21,
		Level_22,
		Level_23,
		Level_24,
		Level_25,
		Level_26,
		Level_27,
		Level_28,
		Level_29,
		Level_30,
		Level_NOT_USED,
}_LEVEL_TypeDef;

typedef enum
{
	Not_Online,
	On_Line,
}_MASTER_STATE;

typedef enum
{
	 Full,
	 No_Full,
}_Charge_TypeDef;

typedef struct
{
	u8 timer_1ms_flag;
	u8 recv_msg_flag;
	u8 send_frame_cnt;
	u8 recv_frame_cnt;
	u8 Loss_Rate;

	_LOCK_STATE    Lock_State;
	_ERR_STATE     Err_State;
	_LEVEL_STATE   Level_State;
	_LEVEL_STATE   Lock_Level_State;
	_LEVEL_TypeDef Level_TypeDef;
	_LEVEL_TypeDef Last_Level_TypeDef;
	// _MASTER_STATE  Master_State;
	u8 Connect;        // Á¬½ÓÖ÷»ú±êÖ¾£¬Ö÷»ú¹Ø»ú 5sºó£¬µ××ù¶Ï¿ªÁ¬½Ó

}_SysInfo_TypeDef;

typedef struct
{
	_LOCK_STATE    Lock_State;
	_ERR_STATE     Err_State;
	_LEVEL_TypeDef Level;
	u8 Connect;
	u8 lock_undisp_flag;  //
	u8 level_undisp_flag; //
	u8 icons_inverse_cnt;
	u8 lock_inverse_cnt;
	u8 level_inverse_cnt;
	u8 idle_mode_flag;

}DISP_DATA_t;



extern _SysInfo_TypeDef SysInfo;
extern uint16_t Sys_Time,LED_Time,Time;
extern uint8_t  Sys_Task;
extern uint16_t RX_Time;
extern uint8_t Reset_Cnt;
extern uint8_t  Interrupt_Cnt;
extern uint16_t Auto_30S_Lock_Cnt;
extern uint16_t OP_ADC;
extern uint16_t Lock_Blink_Time;
extern uint16_t Time_1S,IDLE_120S_Cnt;
extern uint16_t CHARGE_IO,Charge_Time;

extern DISP_DATA_t data_disp;
extern DISP_DATA_t last_data_disp;

////LEDIOï¿½ï¿½ï¿½ï¿½
// #define LED_G_PORT      			 	GPIOB
// #define LED_G_PIN        				GPIO_PIN_8

// #define LED_W_PORT       				GPIOB
// #define LED_W_PIN        				GPIO_PIN_7

// #define LED_G_OFF 			 			GPIO_SetBits(LED_G_PORT, LED_G_PIN);
// #define LED_G_ON 			   			GPIO_ResetBits(LED_G_PORT, LED_G_PIN);

// #define LED_W_OFF 			         	TIM8->CCDAT4 = 1;
// #define LED_W_ON 			   	        TIM8->CCDAT4 = 1400;

#define HW_CE_PIN                		GPIO_PIN_0
#define HW_CE_PORT               		GPIOA
#define HW_IRQ_PIN               		GPIO_PIN_1
#define HW_IRQ_PORT              		GPIOA
#define HW_MOSI_PIN              		GPIO_PIN_2
#define HW_MOSI_PORT             		GPIOA
#define HW_MISO_PIN              		GPIO_PIN_3
#define HW_MISO_PORT             		GPIOA
#define HW_CSN_PIN               		GPIO_PIN_4
#define HW_CSN_PORT              		GPIOA
#define HW_SCK_PIN               		GPIO_PIN_5
#define HW_SCK_PORT              		GPIOA

#define HW_CE_HIGH    					GPIO_SetBits(HW_CE_PORT,   HW_CE_PIN);
#define HW_CE_LOW     					GPIO_ResetBits(HW_CE_PORT, HW_CE_PIN);
#define HW_CSN_HIGH   					GPIO_SetBits(HW_CSN_PORT,   HW_CSN_PIN);
#define HW_CSN_LOW    					GPIO_ResetBits(HW_CSN_PORT, HW_CSN_PIN);

#define HW_SPI_MASTER                 	SPI1
#define HW_SPI_MASTER_CLK             	RCC_APB2_PERIPH_SPI1
#define HW_SPI_MASTER_GPIO_CLK        	RCC_APB2_PERIPH_GPIOA

#define HW_INPUT_EXTI_LINE   			EXTI_LINE1
#define HW_INPUT_PORT_SOURCE 			GPIOA_PORT_SOURCE
#define HW_INPUT_PIN_SOURCE  			GPIO_PIN_SOURCE1
#define HW_INPUT_IRQn        			EXTI4_15_IRQn

//OLEDIOï¿½ï¿½ï¿½Å¶ï¿½ï¿½ï¿½

#define OLED_SCK_PIN               		GPIO_PIN_9
#define OLED_SCK_PORT              		GPIOA

#define OLED_MOSI_PIN              		GPIO_PIN_1
#define OLED_MOSI_PORT             		GPIOB

#define OLED_DC_PIN                		GPIO_PIN_10
#define OLED_DC_PORT               		GPIOA

#define OLED_RST_PIN               		GPIO_PIN_11
#define OLED_RST_PORT              		GPIOA

#define OLED_CS_PIN                		GPIO_PIN_12
#define OLED_CS_PORT               		GPIOA

#define OLED_CS_HIGH    				GPIO_SetBits(OLED_CS_PORT,   OLED_CS_PIN);
#define OLED_CS_LOW     				GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN);

#define OLED_RST_HIGH   				GPIO_SetBits(OLED_RST_PORT,   OLED_RST_PIN);
#define OLED_RST_LOW    				GPIO_ResetBits(OLED_RST_PORT, OLED_RST_PIN);

#define OLED_DC_HIGH    				GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN);
#define OLED_DC_LOW     				GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN);

#define OLED_SPI_MASTER                 SPI2
#define OLED_SPI_MASTER_CLK             RCC_APB2_PERIPH_SPI2
#define OLED_SPI_MASTER_GPIO_CLK        RCC_APB2_PERIPH_GPIOA
#define OLED_SPI_MASTER_DMA             DMA  7
#define OLED_SPI_MASTER_DMA_CLK         RCC_AHB_PERIPH_DMA
#define OLED_SPI_MASTER_TX_DMA_CHANNEL  DMA_CH3
#define OLED_SPI_MASTER_TX_DMA_FLAG     DMA_FLAG_TC3


#define ADD_KEY_PORT                    GPIOB
#define ADD_KEY_PIN                     GPIO_PIN_5
#define DEC_KEY_PORT                    GPIOB
#define DEC_KEY_PIN                     GPIO_PIN_6
//¿ªÉÏÀ­µç×è£¬µÍµçÆ½Îª°´¼ü°´ÏÂ
#define ADD_KEY		                    GPIO_ReadInputDataBit(ADD_KEY_PORT, ADD_KEY_PIN)
#define IS_ADD_KEY_PRESS()		        (RESET==GPIO_ReadInputDataBit(ADD_KEY_PORT, ADD_KEY_PIN))
//¿ªÉÏÀ­µç×è£¬µÍµçÆ½Îª°´¼ü°´ÏÂ
#define DEC_KEY			                GPIO_ReadInputDataBit(DEC_KEY_PORT, DEC_KEY_PIN)
#define IS_DEC_KEY_PRESS()		        (RESET==GPIO_ReadInputDataBit(DEC_KEY_PORT, DEC_KEY_PIN))

#define KEY_INPUT_EXTI_LINE   			EXTI_LINE6
#define KEY_INPUT_PORT_SOURCE 			GPIOB_PORT_SOURCE
#define KEY_INPUT_PIN_SOURCE  			GPIO_PIN_SOURCE6
#define KEY_INPUT_IRQn        			EXTI4_15_IRQn
//ï¿½ï¿½ï¿½ï¿½IOï¿½ï¿½ï¿½ï¿½
// #define OP_ADC_PORT      				GPIOB
// #define OP_ADC_PIN       				GPIO_PIN_2
// #define CHECK_PORT       				GPIOB
// #define CHECK_PIN        				GPIO_PIN_3
// #define CHECK_IN         				GPIO_ReadInputDataBit(CHECK_PORT, CHECK_PIN)

// #define OUTPUT_EN_PORT   				GPIOA
// #define OUTPUT_EN_PIN    				GPIO_PIN_15
// #define CHARGE_OFF 		 				GPIO_ResetBits(OUTPUT_EN_PORT, OUTPUT_EN_PIN);
// #define CHARGE_ON 		 				GPIO_SetBits  (OUTPUT_EN_PORT, OUTPUT_EN_PIN);


#define ABFM03_ADDR_NUM             	14//12
#define KEY_SHORT_TIME              	20//100

#endif

