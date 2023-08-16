
#include "pwm.h"
ADC_InitType ADC_InitStructure;
uint16_t ADCConvertedValue[5];
uint16_t ADC_GetData(uint8_t ADC_Channel);
void MX_ADC_Init(void);

void ADC_RCC_Configuration(void)
{
    ErrorStatus HSIStartUpStatus;
    /* Enable GPIOC clocks */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    /* Enable ADC clocks */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
    /* enable ADC 1M clock */
    RCC_EnableHsi(ENABLE);
    /* Wait til1 HSI is ready*/
    HSIStartUpStatus = RCC_WaitHsiStable();
    if(HSIStartUpStatus == SUCCESS)
    {
    }
    else
    {
        /* If HSI fails to start-up, the application will have wrong clock configuration. User can add here some code to deal with this error*/
        /* Go to infinitel1oop*/
        while(1)
        {
        }
    }
    RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8);
    /* RCC_ADCHCLK_DIV16*/
    ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB, RCC_ADCHCLK_DIV16);
}

void ADC_GPIO_Configuration(void)
{
    // GPIO_InitType GPIO_InitStructure;

    // GPIO_InitStruct(&GPIO_InitStructure);
    // /* Configure PA.01 as analog input -------------------------*/
    // GPIO_InitStructure.Pin       = OP_ADC_PIN;
    // GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    // GPIO_InitPeripheral(OP_ADC_PORT, &GPIO_InitStructure);
}
void ADC_Initial(void)
{
	    /* ADC configuration ------------------------------------------------------*/
    ADC_InitStructure.MultiChEn      = DISABLE;
    ADC_InitStructure.ContinueConvEn = DISABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = 10;
    ADC_Init(ADC, &ADC_InitStructure);
//    /* ADC1 regular channel13 configuration */
//    ADC_ConfigRegularChannel(ADC, ADC_CH_13_PC2, 1, ADC_SAMP_TIME_55CYCLES5);
//    /* Enable ADC DMA */
//    ADC_EnableDMA(ADC, ENABLE);

    /* Enable ADC */
    ADC_Enable(ADC, ENABLE);
    /* Check ADC Ready */
    while(ADC_GetFlagStatusNew(ADC,ADC_FLAG_RDY) == RESET)
        ;
    while(ADC_GetFlagStatusNew(ADC,ADC_FLAG_PD_RDY))
        ;
}
uint16_t ADC_GetData(uint8_t ADC_Channel)
{
    uint16_t dat;
    ADC_ConfigRegularChannel(ADC, ADC_Channel, 1, ADC_SAMP_TIME_56CYCLES5);
    /* Start ADC Software Conversion */
    ADC_EnableSoftwareStartConv(ADC,ENABLE);
    while(ADC_GetFlagStatus(ADC,ADC_FLAG_ENDC)==0){
    }
    ADC_ClearFlag(ADC,ADC_FLAG_ENDC);
    ADC_ClearFlag(ADC,ADC_FLAG_STR);
    dat=ADC_GetDat(ADC);
    return dat;
}
void MX_ADC_Init(void)
{
	ADC_RCC_Configuration();
	ADC_GPIO_Configuration();
	ADC_Initial();
}



