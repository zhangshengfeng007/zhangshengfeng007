
#include "gpio.h"

#define SYSCLK_USE_HSI     0
#define SYSCLK_USE_HSE     1
#define SYSCLK_USE_HSI_PLL 2
#define SYSCLK_USE_HSE_PLL 3
#define SYSCLK_USE_LSE     4
#define SYSCLK_USE_LSI     5

void MX_GPIO_Init(void);

void MX_GPIO_Init(void)
{

		GPIO_InitType GPIO_InitStructure;

		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

		// GPIO_InitStruct(&GPIO_InitStructure);
		// GPIO_InitStructure.Pin = LED_G_PIN;
		// GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_LOW;
		// GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		// GPIO_InitStruct(&GPIO_InitStructure);
		// GPIO_InitStructure.Pin = OUTPUT_EN_PIN;
		// GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_LOW;
		// GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

	  	GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin =  DEC_KEY_PIN|ADD_KEY_PIN;   //|CHECK_PIN
		GPIO_InitStructure.GPIO_Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_LOW;
		GPIO_InitStructure.GPIO_Pull  = GPIO_NO_PULL;  //  GPIO_PULL_UP： 因为外部有上拉电阻，所以不用内部电阻
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

}



