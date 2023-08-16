
#include "exit.h"

void KeyInput_Exti_Init(void);
void HW_IRQ_Exti_Init(void);

void KeyInput_Exti_Init(void)
{
//    GPIO_InitType GPIO_InitStructure;
//    EXTI_InitType EXTI_InitStructure;
//    NVIC_InitType NVIC_InitStructure;

//    /* Enable the GPIO Clock */
//    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);


//    /*Configure the GPIO pin as input floating*/
//		GPIO_InitStruct(&GPIO_InitStructure);
//		GPIO_InitStructure.Pin          = KEYA_PIN;
//	  GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_INPUT;
//		GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
//		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

//    /*Configure key EXTI Line to key input Pin*/
//    GPIO_ConfigEXTILine(KEY_INPUT_PORT_SOURCE, KEY_INPUT_PIN_SOURCE);

//    /*Configure key EXTI line*/
//    EXTI_InitStructure.EXTI_Line    = KEY_INPUT_EXTI_LINE;
//    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_InitPeripheral(&EXTI_InitStructure);

//    /*Set key input interrupt priority*/
//    NVIC_InitStructure.NVIC_IRQChannel                   = KEY_INPUT_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}

void HW_IRQ_Exti_Init(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);


    /*Configure the GPIO pin as input floating*/
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin          = HW_IRQ_PIN;
	  GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_INPUT;
		GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
		GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(HW_INPUT_PORT_SOURCE, HW_INPUT_PIN_SOURCE);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = HW_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = HW_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

