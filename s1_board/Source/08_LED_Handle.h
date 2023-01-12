#ifndef __LED_HANDLE_H
#define __LED_HANDLE_H

#define CodeOne  0x7f
#define CodeZero 0x70

#define LED1_ON  HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
#define LED1_OFF HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)
#define LED2_ON  HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define LED2_OFF HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
#define LED3_ON  HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define LED3_OFF HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define LED_ALL_OFF  LED1_OFF;LED2_OFF;LED3_OFF;

extern uint8_t Led_Colour[9];
extern void RGB_LED_Init(void);
extern void Set_Color_Handle(uint8_t R,uint8_t G,uint8_t B,uint8_t num);
extern void LCD_output_transf(uint8_t *data);
extern void LED_display(void);
extern void LED_Display_Error(void);
#endif

