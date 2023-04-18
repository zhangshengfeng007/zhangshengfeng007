/***********************************************************
* @file    drv_gpio.c
* @brief  gpio程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_gpio.h"

/*
*************************************************************
* 功能说明: GPIO初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void GPIO_Init(void)
{
	/* enable the  GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	Gpio_Reset();
	//OUTPUT
	gpio_mode_set(LASER_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LASER_PIN);
	gpio_output_options_set(LASER_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LASER_PIN);
	LED_LASER_OFF;

	gpio_mode_set(LED_BAT_LOW_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BAT_LOW_PIN);
	gpio_output_options_set(LED_BAT_LOW_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_BAT_LOW_PIN);
	LED_BAT_LOW_OFF;

	gpio_mode_set(LED_BAT_FULL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BAT_FULL_PIN);
	gpio_output_options_set(LED_BAT_FULL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_BAT_FULL_PIN);
	LED_BAT_FULL_OFF;

	gpio_mode_set(LED_1D_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_1D_PIN);
	gpio_output_options_set(LED_1D_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_1D_PIN);
	LED_1D_OFF;

	gpio_mode_set(LED_2D_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_2D_PIN);
	gpio_output_options_set(LED_2D_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_2D_PIN);
	LED_2D_OFF;

	gpio_mode_set(LED_3D_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_3D_PIN);
	gpio_output_options_set(LED_3D_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_3D_PIN);
	LED_3D_OFF;

	gpio_mode_set(CHARGE_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CHARGE_EN_PIN);
	gpio_output_options_set(CHARGE_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, CHARGE_EN_PIN);
    CHARGE_EN_OFF;

	gpio_mode_set(PWM1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWM1_PIN);
	gpio_output_options_set(PWM1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PWM1_PIN);
	PWM1_OFF;

	// gpio_mode_set(PWM_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWM_EN_PIN);
	// gpio_output_options_set(PWM_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, PWM_EN_PIN);

	/*configure PA6(TIMER_15 CH0) as alternate function*/
	gpio_mode_set(PWM_EN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_EN_PIN);
	gpio_output_options_set(PWM_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_EN_PIN);
	gpio_af_set(PWM_EN_PORT, GPIO_AF_5, PWM_EN_PIN);

	gpio_mode_set(POWER_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, POWER_PIN);
	gpio_output_options_set(POWER_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, POWER_PIN);

	// Gpio_Reset();
}

/*
*************************************************************
* 功能说明: GPIO复位
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Gpio_Reset(void)
{
	LED_LASER_OFF;
	LED_BAT_LOW_OFF;
	LED_BAT_FULL_OFF;
	LED_1D_OFF;
	LED_2D_OFF;
	LED_3D_OFF;
	CHARGE_EN_OFF;
	PWM1_OFF;

}




