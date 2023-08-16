
#include "spi.h"

void MX_SPI2_Init(void);
void SPI_RCC_Configuration(void);
void SPI_GPIO_Configuration(void);

SPI_InitType SPI_InitStructure;
DMA_InitType DMA_InitStructure;

#define BUFFER_SIZE    32
#define CRC_POLYNOMIAL 7

void SPI_RCC_Configuration(void)
{
    /* Enable peripheral clocks --------------------------------------------------*/

    /* Enable GPIO clock for SPIy and SPIz */
    RCC_EnableAPB2PeriphClk(OLED_SPI_MASTER_GPIO_CLK | HW_SPI_MASTER_GPIO_CLK  | RCC_APB2_PERIPH_AFIO, ENABLE);//

    /* Enable SPIy and SPIz Periph clock */
    RCC_EnableAPB2PeriphClk(OLED_SPI_MASTER_CLK | HW_SPI_MASTER_CLK , ENABLE);//
	  //RCC_EnableAPB2PeriphClk(OLED_SPI_MASTER_GPIO_CLK | OLED_SPI_MASTER_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
}


void SPI_GPIO_Configuration(void)
{
    	GPIO_InitType GPIO_InitStructure;
	

    /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
    /* Confugure SCK , MISO and MOSI pins as Alternate Function Push Pull */
			GPIO_InitStructure.Pin        = OLED_SCK_PIN ;
			GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
			GPIO_InitStructure.GPIO_Pull  = GPIO_NO_PULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI2;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.Pin        = OLED_MOSI_PIN;
			GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
			GPIO_InitStructure.GPIO_Pull  = GPIO_NO_PULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.GPIO_Alternate = GPIO_AF8_SPI2;
			GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	
	    //OLEDspi����������
			RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
		
			GPIO_InitStruct(&GPIO_InitStructure);
			GPIO_InitStructure.Pin = OLED_DC_PIN | OLED_RST_PIN |OLED_CS_PIN;//|LCD_SCLK_PIN|LCD_SDIN_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.GPIO_Pull  = GPIO_PULL_UP;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);	
      //����HW����	
			GPIO_InitStruct(&GPIO_InitStructure);
			GPIO_InitStructure.Pin        = HW_SCK_PIN; // SCK
			GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
			GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI1;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStructure.Pin = HW_MOSI_PIN;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStructure.Pin       = HW_MISO_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStructure.Pin       = HW_CE_PIN | HW_CSN_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStruct(&GPIO_InitStructure);

}

void OLED_SPI_Configuration(void)
{
	  SPI_InitStructure.DataDirection = SPI_DIR_SINGLELINE_TX;//   //����SPI�������˫�������ģʽ:SPI����Ϊ��
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;            //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;            //ѡ���˴���ʱ�ӵ���̬:����ʱ�ӵ�
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;     //���ݲ���(����)�ڵ�1��ʱ����
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;              //������� NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_8;      //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.CRCPoly       = 7;                         //CRCֵ����Ķ���ʽ
    SPI_Init(OLED_SPI_MASTER, &SPI_InitStructure);

    /* Enable SPI_MASTER TE interrupt */
//    SPI_I2S_EnableInt(OLED_SPI_MASTER, SPI_I2S_INT_TE, ENABLE);
    /* Enable SPI_MASTER */
    SPI_Enable(OLED_SPI_MASTER, ENABLE);
}
void HW2000_SPI_Configuration(void)
{
	SPI_InitType SPI_InitStructure;
	
	  SPI_InitStruct(&SPI_InitStructure);
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_16;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = 7;
    SPI_Init(HW_SPI_MASTER, &SPI_InitStructure);

    SPI_Enable(HW_SPI_MASTER, ENABLE);
} 
///* SPI2 init function */
void MX_SPI2_Init(void)
{
		SPI_RCC_Configuration();
		SPI_GPIO_Configuration();
		OLED_SPI_Configuration();
		HW2000_SPI_Configuration();
}

