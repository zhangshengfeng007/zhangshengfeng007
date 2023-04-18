/***********************************************************
* @file    drv_adc.c
* @brief  adc驱动程序
* @version V1.0
* @date    2022.11.21
***********************************************************/
#include "drv_adc.h"
#include "drv_delay.h"

volatile uint16_t adc_value[4];



/*
*************************************************************
* 功能说明: ADC时钟配置
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_Clock_Config(void)
{
	/* enable ADC clock */
	rcu_periph_clock_enable(RCU_ADC);
	/* enable DMA clock */
	rcu_periph_clock_enable(RCU_DMA);
	/* config ADC clock */
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
}

/*
*************************************************************
* 功能说明: ADC GPIO配置
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_Gpio_Config(void)
{
	/* enable GPIOC clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	/* config the GPIO as analog mode */
	gpio_mode_set(RF_NTC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, RF_NTC_PIN);
	gpio_mode_set(BAT_NTC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BAT_NTC_PIN);
	gpio_mode_set(BAT_ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BAT_ADC_PIN);
	gpio_mode_set(POW_ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, POW_ADC_PIN);
}

/*
*************************************************************
* 功能说明: ADC DMA配置
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_Dma_Config(void)
{
	/* ADC_DMA_channel configuration */
	dma_parameter_struct dma_data_parameter;

	/* ADC DMA_channel configuration */
	dma_deinit(DMA_CH0);

	/* initialize DMA single data mode */
	dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA);
	dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
	dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_data_parameter.number       = 4U;
	dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
	dma_init(DMA_CH0, &dma_data_parameter);

	dma_circulation_enable(DMA_CH0);

	/* enable DMA channel */
	dma_channel_enable(DMA_CH0);
}

/*
*************************************************************
* 功能说明: ADC配置
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_Config(void)
{
	/* ADC continuous function enable */
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
	/* ADC scan function enable */
	adc_special_function_config(ADC_SCAN_MODE, ENABLE);
	/* ADC data alignment config */
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
	/* ADC channel length config */
	adc_channel_length_config(ADC_REGULAR_CHANNEL, 4U);

	/* ADC regular channel config */
	adc_regular_channel_config(0, ADC_CHANNEL_2, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(1, ADC_CHANNEL_3, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(2, ADC_CHANNEL_4, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(3, ADC_CHANNEL_5, ADC_SAMPLETIME_55POINT5);

	/* ADC trigger config */
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

	/* enable ADC interface */
	adc_enable();
	Delay_ms(10);
	/* ADC calibration and reset calibration */
	adc_calibration_enable();
	Delay_ms(10);

	/* ADC DMA function enable */
	adc_dma_mode_enable();
	/* ADC software trigger enable */
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}

/*
*************************************************************
* 功能说明: Adc 恢复默认初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_DeInit(void)
{
	// adc_disable();
	// adc_dma_mode_disable();
	// gpio_mode_set(BAT_ADC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BAT_ADC_PIN);
	adc_deinit();
	dma_deinit(DMA_CH0);
}


void Adc_ReInit(void)
{
	// gpio_mode_set(BAT_ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BAT_ADC_PIN);
	// adc_enable();
	// adc_dma_mode_enable();
	Adc_Dma_Config();
	Adc_Config();
}


/*
*************************************************************
* 功能说明: Adc初始化
* 形    参: 无
* 返 回 值: 无
*************************************************************
*/
void Adc_Init(void)
{
	Adc_Clock_Config();
	Adc_Gpio_Config();
	Adc_Dma_Config();
	Adc_Config();
}

