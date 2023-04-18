#include "drv_delay.h"

/*
*************************************************************
* ����˵��: ��ʼ����ʱ����
* ��    ��: ��
* �� �� ֵ: ��
*************************************************************
*/
void Delay_Init(void)
{
	/* ----------------------------------------------------------------------------
	TIMER Configuration:
	TIMERCLK = 72M.
	---------------------------------------------------------------------------- */
	timer_parameter_struct timer_initpara;

	/* enable the peripherals clock */
	rcu_periph_clock_enable(RCU_TIMER16);

	/* deinit a TIMER */
	timer_deinit(TIMER16);
	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&timer_initpara);
	/* TIMER configuration */
	timer_initpara.prescaler         	= 71;
	timer_initpara.alignedmode		= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection	= TIMER_COUNTER_UP;
	timer_initpara.period			= 10000;
	timer_initpara.clockdivision		= TIMER_CKDIV_DIV1;
	timer_init(TIMER16, &timer_initpara);

	/* clear channel 0 interrupt bit */
	timer_interrupt_flag_clear(TIMER16, TIMER_INT_FLAG_UP);
	/* enable a TIMER */
	timer_enable(TIMER16);
	
}								    

/*
*************************************************************
* ����˵��: us����ʱ
* ��    ��: nus ��Ҫ��ʱ��ʱ��
* �� �� ֵ: ��
*************************************************************
*/
void Delay_us(uint16_t nus)
{	
	uint16_t ticks;
	uint16_t told = 0,tnow = 0,tcnt = 0;
	uint16_t reload = 10000;	//��װ��ֵ	
	
	ticks = nus; 						
	told = timer_counter_read(TIMER16);	//��ʼ����ֵ

	while(1)
	{
		tnow = timer_counter_read(TIMER16);	//��ǰ����ֵ
		if(tnow != told)
		{	    
			if(tnow > told)
				tcnt += tnow - told;	
			else 
				tcnt += reload - told + tnow; 

			told = tnow;
				
			if(tcnt >= ticks)
				break;
		}  
	};
}  

/*
*************************************************************
* ����˵��: ms����ʱ
* ��    ��: nms��Ҫ��ʱ��ʱ��
* �� �� ֵ: ��
*************************************************************
*/
void Delay_ms(uint16_t nms)
{
	uint16_t i = 0;
	
	for(i = 0; i < nms; i++){
		Delay_us(1000);
	}
}



