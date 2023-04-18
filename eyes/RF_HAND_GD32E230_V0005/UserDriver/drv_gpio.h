#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Exported macro ------------------------------------------------------------*/

#define  LED_LASER_ON    		gpio_bit_write(LASER_PORT, LASER_PIN,SET)
#define  LED_LASER_OFF   		gpio_bit_write(LASER_PORT, LASER_PIN,RESET)

#define  LED_BAT_LOW_ON    	    gpio_bit_write(LED_BAT_LOW_PORT, LED_BAT_LOW_PIN,RESET)
#define  LED_BAT_LOW_OFF   	    gpio_bit_write(LED_BAT_LOW_PORT, LED_BAT_LOW_PIN,SET)

#define  LED_BAT_FULL_ON    	gpio_bit_write(LED_BAT_FULL_PORT, LED_BAT_FULL_PIN,RESET)
#define  LED_BAT_FULL_OFF   	gpio_bit_write(LED_BAT_FULL_PORT, LED_BAT_FULL_PIN,SET)

#define  LED_1D_ON    		    gpio_bit_write(LED_1D_PORT, LED_1D_PIN,RESET)
#define  LED_1D_OFF   		    gpio_bit_write(LED_1D_PORT, LED_1D_PIN,SET)
#define  LED_1D_TOGGLE  		gpio_bit_toggle(LED_1D_PORT, LED_1D_PIN)

#define  LED_2D_ON    		    gpio_bit_write(LED_2D_PORT, LED_2D_PIN,RESET)
#define  LED_2D_OFF   		    gpio_bit_write(LED_2D_PORT, LED_2D_PIN,SET)
#define  LED_2D_TOGGLE  		gpio_bit_toggle(LED_2D_PORT, LED_2D_PIN)

#define  LED_3D_ON    		    gpio_bit_write(LED_3D_PORT, LED_3D_PIN,RESET)
#define  LED_3D_OFF   		    gpio_bit_write(LED_3D_PORT, LED_3D_PIN,SET)
#define  LED_3D_TOGGLE   	    gpio_bit_toggle(LED_3D_PORT, LED_3D_PIN)

#define  CHARGE_EN_ON    	    gpio_bit_write(CHARGE_EN_PORT, CHARGE_EN_PIN,RESET)
#define  CHARGE_EN_OFF   	    gpio_bit_write(CHARGE_EN_PORT, CHARGE_EN_PIN,SET)

#define  PWM1_ON    			gpio_bit_write(PWM1_PORT, PWM1_PIN,SET)
#define  PWM1_OFF   			gpio_bit_write(PWM1_PORT, PWM1_PIN,RESET)

#define  PWM_EN_ON    		//gpio_bit_write(PWM_EN_PORT, PWM_EN_PIN,SET)
#define  PWM_EN_OFF   		//gpio_bit_write(PWM_EN_PORT, PWM_EN_PIN,RESET)

#define  POW_ON    			    gpio_bit_write(POWER_PORT, POWER_PIN,SET)
#define  POW_OFF   			    gpio_bit_write(POWER_PORT, POWER_PIN,RESET)


/* Exported functions ------------------------------------------------------- */
void GPIO_Init(void);
void Gpio_Reset(void);


#endif

