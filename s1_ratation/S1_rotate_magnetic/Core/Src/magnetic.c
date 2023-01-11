/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "magnetic.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// 删除一部分信息，测试上传功能
/* USER CODE BEGIN PV */

void Slope1_magnetic1_out(void);
void Slope2_magnetic2_out(void);
void Vertical_magnetic_out(void);

magentic_data_t mag_data_arr[]={
	{SLOPE_TYPE1, Slope1_magnetic1_out, 1},
	{SLOPE_TYPE2, Slope2_magnetic2_out, 1},
	{VERTICAL_TYPE, Vertical_magnetic_out, 1},
};


//magentic_data_t mag_data_arr[]={
//	{VERTICAL_TYPE, Vertical_magnetic_out, 1},
//	{VERTICAL_TYPE, Vertical_magnetic_out, 1},
//	{VERTICAL_TYPE, Vertical_magnetic_out, 1},
//};



mytest_t my_test;
magnetic_type_e mag_type;

extern port_pin_t my_pin_array[];
extern sys_info_t sys_info;


void pin_value_set(port_pin_t *port, pin_type_e pin_type)
{
	//my_test.pin_type = pin_type;
	my_test.port_pin.p_port = port->p_port;
	my_test.port_pin.p_pin = port->p_pin;
	my_test.port_pin.n_port = port->n_port;
	my_test.port_pin.n_pin = port->n_pin;


	switch (pin_type)
	{
		case P_PIN_ON:
		{
			if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(port->p_port, port->p_pin))
			{
				HAL_GPIO_WritePin(port->p_port, port->p_pin, GPIO_PIN_SET);
			}
			break;
		}
		default:
		{
		
			if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(port->n_port, port->n_pin))
			{
			   HAL_GPIO_WritePin(port->n_port, port->n_pin, GPIO_PIN_SET);
			}
			break;
		}
	}
}



void pin_value_clear(port_pin_t *port, pin_type_e pin_type)
{
	//my_test.pin_type = pin_type;
	my_test.port_pin.p_port = port->p_port;
	my_test.port_pin.p_pin = port->p_pin;
	my_test.port_pin.n_port = port->n_port;
	my_test.port_pin.n_pin = port->n_pin;

	switch (pin_type)
	{
		case P_PIN_OFF:
		{
			HAL_GPIO_WritePin(port->p_port, port->p_pin, GPIO_PIN_RESET);
			break;
		}
		case N_PIN_OFF:
		{
			HAL_GPIO_WritePin(port->n_port, port->n_pin, GPIO_PIN_RESET);
			break;
		}
		default:
		{
			HAL_GPIO_WritePin(port->n_port, port->n_pin, GPIO_PIN_RESET);
			break;
		}
	}
}


 void MAG_value_out(magen_pin_e magen_pin, pin_type_e pin_type)
{

	port_pin_t *port_pointer;
	uint8_t index;

	my_test.magen = magen_pin;

	index = find_port_index(magen_pin);
	port_pointer = &my_pin_array[index];
	pin_value_set(port_pointer, pin_type);
}


void MAG_value_off(magen_pin_e magen_pin, pin_type_e pin_type)
{

	port_pin_t *port_pointer;
	uint8_t index;

	my_test.magen = magen_pin;

	index = find_port_index(magen_pin);
	port_pointer = &my_pin_array[index];
	pin_value_clear(port_pointer, pin_type);
}


void all_p_port_on(void)
{
	MAG_value_out(MAG_A1, P_PIN_ON);
	MAG_value_out(MAG_A2, P_PIN_ON);
	MAG_value_out(MAG_A3, P_PIN_ON);
	MAG_value_out(MAG_A4, P_PIN_ON);
	MAG_value_out(MAG_A5, P_PIN_ON);
	MAG_value_out(MAG_A6, P_PIN_ON);
	MAG_value_out(MAG_A7, P_PIN_ON);
//	
	MAG_value_out(MAG_B1, P_PIN_ON);
	MAG_value_out(MAG_B2, P_PIN_ON);
	MAG_value_out(MAG_B3, P_PIN_ON);
	MAG_value_out(MAG_B4, P_PIN_ON);
	MAG_value_out(MAG_B5, P_PIN_ON);
	
	
	MAG_value_out(MAG_C1, P_PIN_ON);
	MAG_value_out(MAG_C2, P_PIN_ON);
	MAG_value_out(MAG_C3, P_PIN_ON);
	MAG_value_out(MAG_C4, P_PIN_ON);
	MAG_value_out(MAG_C5, P_PIN_ON);
	MAG_value_out(MAG_C6, P_PIN_ON);
	MAG_value_out(MAG_C7, P_PIN_ON);
}



void all_n_port_off(void)
{

	MAG_value_off(MAG_A1, N_PIN_OFF);
	MAG_value_off(MAG_A2, N_PIN_OFF);
	MAG_value_off(MAG_A3, N_PIN_OFF);
	MAG_value_off(MAG_A4, N_PIN_OFF);
	MAG_value_off(MAG_A5, N_PIN_OFF);
	MAG_value_off(MAG_A6, N_PIN_OFF);
	MAG_value_off(MAG_A7, N_PIN_OFF);
	
	MAG_value_off(MAG_B1, N_PIN_OFF);
	MAG_value_off(MAG_B2, N_PIN_OFF);
	MAG_value_off(MAG_B3, N_PIN_OFF);
	MAG_value_off(MAG_B4, N_PIN_OFF);
	MAG_value_off(MAG_B5, N_PIN_OFF);

	MAG_value_off(MAG_C1, N_PIN_OFF);
	MAG_value_off(MAG_C2, N_PIN_OFF);
	MAG_value_off(MAG_C3, N_PIN_OFF);
	MAG_value_off(MAG_C4, N_PIN_OFF);
	MAG_value_off(MAG_C5, N_PIN_OFF);
	MAG_value_off(MAG_C6, N_PIN_OFF);
	MAG_value_off(MAG_C7, N_PIN_OFF);
}


void all_p_on_n_off(void)
{
	all_n_port_off();
	all_p_port_on();
}

//-------------------------------

void Slope1_select_p_off(void)
{
	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1,A2,C2   MAG_A1
		{					
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_A2, P_PIN_OFF);
			MAG_value_off(MAG_C2, P_PIN_OFF);
			break;
		}
		case MAG_STEP2:  // N: A1, A3,
						 // N:  B1
		{				//  N:C1, C3
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_B1, P_PIN_OFF);
			MAG_value_off(MAG_C1, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);
			break;
		}
	
		case MAG_STEP3:  // N: A2 A4
		{		         // N: B2
						//  N: C2 C4	
			MAG_value_off(MAG_A2, P_PIN_OFF);
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_B2, P_PIN_OFF);
			MAG_value_off(MAG_C2, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);
			
			break;       
		}
		case MAG_STEP4:  // N:A3  A5
		{            	  // N: B3
						  // N: C3 C5
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_B3, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);
		
			break;
		}
			
		case MAG_STEP5:     // N:A4 A6
		{				 	// N:B4
							// N:C4 C6
				
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_B4, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);	
			break;
		}
		case MAG_STEP6:  	// N:A5 A7
		{                	// N:B5
							 //N: C5 C7
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_A7, P_PIN_OFF);
			MAG_value_off(MAG_B5, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);
			MAG_value_off(MAG_C7, P_PIN_OFF);	
			
			break;
		}
		case MAG_STEP7:
		{					// N:A6 A7
							// N:C6
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_A7, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Slope1_select_n_on(void)
{
	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1,A2,C2   MAG_A1
		{					
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_A2, N_PIN_ON);
		    MAG_value_out(MAG_C2, N_PIN_ON);
			break;
		}
		case MAG_STEP2:  // N: A1, A3,
						 // N:  B1
		{				//  N:C1, C3
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_A3, N_PIN_ON);
			
			MAG_value_out(MAG_B1, N_PIN_ON);
			MAG_value_out(MAG_C1, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);
			break;
		}
	
		case MAG_STEP3:  // N: A2 A4
		{		         // N: B2
						//  N: C2 C4	
			MAG_value_out(MAG_A2, N_PIN_ON);
			MAG_value_out(MAG_A4, N_PIN_ON);
			
			MAG_value_out(MAG_B2, N_PIN_ON);
			MAG_value_out(MAG_C2, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);
			
			break;       
		}
		case MAG_STEP4:  // N:A3  A5
		{            	  // N: B3
						  // N: C3 C5
			MAG_value_out(MAG_A3, N_PIN_ON);
			MAG_value_out(MAG_A5, N_PIN_ON);
			
			MAG_value_out(MAG_B3, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);
		
			break;
		}
			
		case MAG_STEP5:     // N:A4 A6
		{				 	// N:B4
							// N:C4 C6
				
			MAG_value_out(MAG_A4, N_PIN_ON);
			MAG_value_out(MAG_A6, N_PIN_ON);
			
			MAG_value_out(MAG_B4, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);	
			break;
		}
		case MAG_STEP6:  	// N:A5 A7
		{                	// N:B5
							 //N: C5 C7
			MAG_value_out(MAG_A5, N_PIN_ON);
			MAG_value_out(MAG_A7, N_PIN_ON);
			
			MAG_value_out(MAG_B5, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);
			MAG_value_out(MAG_C7, N_PIN_ON);	
			
			break;
		}
		case MAG_STEP7:
		{					// N:A6 A7
							// N:C6
			MAG_value_out(MAG_A6, N_PIN_ON);
			MAG_value_out(MAG_A7, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);
			break;
		}
		default:
		{
			break;
		}
	}
}


/*
	 A2-A3-A4-A5-A6
  A1-               -A7
     B1-B2-B3-B4-B5
  C1-               -C7
	 C2-C3-C4-C5-C6
*/


void Slope1_magnetic1_out(void)
{
	
	all_p_on_n_off();
	Slope1_select_p_off();
	delay_1ms_timer_start();

}





void Slope2_select_p_off(void)
{

	
	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A2
		{			    	//  N:C1 C2
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_C1, P_PIN_OFF);
			MAG_value_off(MAG_C2, P_PIN_OFF);
			break;
		}
		case MAG_STEP2:  // N: A1, A3,
						 // N:  B1
		{				//  N:C1, C3
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_B1, P_PIN_OFF);
			MAG_value_off(MAG_C1, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);
			break;
		}
	
		case MAG_STEP3:  // N: A2 A4
		{		         // N: B2
						//  N: C2 C4	
			MAG_value_off(MAG_A2, P_PIN_OFF);
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_B2, P_PIN_OFF);
			MAG_value_off(MAG_C2, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);	
			
			break;       
		}
		case MAG_STEP4:  // N:A3  A5
		{            	  // N: B3
						  // N: C3 C5
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_B3, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);
				
			break;
		}
			
		case MAG_STEP5:     // N:A4 A6
		{				 	// N:B4
							// N:C4 C6
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_B4, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);		
			break;
		}
		case MAG_STEP6:  	// N:A5 A7
		{                	// N:B5
							 //N: C5 C7
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_A7, P_PIN_OFF);
			MAG_value_off(MAG_B5, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);
			MAG_value_off(MAG_C7, P_PIN_OFF);		
			
			break;
		}
		case MAG_STEP7:
		{					// N:A6 
							// N:C6,C7
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);
			MAG_value_off(MAG_C7, P_PIN_OFF);
			break;
		}
		default:
		{
		
			break;
		}
	}
}


void Slope2_select_n_on(void)
{
	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A2
		{			    	//  N:C1 C2
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_C1, N_PIN_ON);
			MAG_value_out(MAG_C2, N_PIN_ON);
			break;
		}
		case MAG_STEP2:  // N: A1, A3,
						 // N:  B1
		{				//  N:C1, C3
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_A3, N_PIN_ON);
			MAG_value_out(MAG_B1, N_PIN_ON);
			MAG_value_out(MAG_C1, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);
			break;
		}
	
		case MAG_STEP3:  // N: A2 A4
		{		         // N: B2
						//  N: C2 C4	
			MAG_value_out(MAG_A2, N_PIN_ON);
			MAG_value_out(MAG_A4, N_PIN_ON);
			MAG_value_out(MAG_B2, N_PIN_ON);
			MAG_value_out(MAG_C2, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);	
			
			break;       
		}
		case MAG_STEP4:  // N:A3  A5
		{            	  // N: B3
						  // N: C3 C5
			MAG_value_out(MAG_A3, N_PIN_ON);
			MAG_value_out(MAG_A5, N_PIN_ON);
			MAG_value_out(MAG_B3, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);
				
			break;
		}
			
		case MAG_STEP5:     // N:A4 A6
		{				 	// N:B4
							// N:C4 C6
			MAG_value_out(MAG_A4, N_PIN_ON);
			MAG_value_out(MAG_A6, N_PIN_ON);
			MAG_value_out(MAG_B4, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);		
			break;
		}
		case MAG_STEP6:  	// N:A5 A7
		{                	// N:B5
							 //N: C5 C7
			MAG_value_out(MAG_A5, N_PIN_ON);
			MAG_value_out(MAG_A7, N_PIN_ON);
			MAG_value_out(MAG_B5, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);
			MAG_value_out(MAG_C7, N_PIN_ON);		
			
			break;
		}
		case MAG_STEP7:
		{					// N:A6 
							// N:C6,C7
			MAG_value_out(MAG_A6, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);
			MAG_value_out(MAG_C7, N_PIN_ON);
			break;
		}
		default:
		{
		
			break;
		}
	}
}


void Slope2_magnetic2_out(void)
{
	all_p_on_n_off();
	Slope2_select_p_off();
	delay_1ms_timer_start();
}

/* VERTICAL_TYPE   �������� ��� 1--2--3--4--5--6--7 */

void Vertical_select_p_off(void)
{

	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1 C2
		{			   		 //  
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_C1, P_PIN_OFF);
			break;
		}
		case MAG_STEP2:  // N:  A2, B1, C2,
		{				
			MAG_value_off(MAG_A2, P_PIN_OFF);
			MAG_value_off(MAG_B1, P_PIN_OFF);
			MAG_value_off(MAG_C2, P_PIN_OFF);
			break;
		}
	
		case MAG_STEP3:  // N: A3 -B2 - C3
		{		         
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_B2, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);			
			break;       
		}
		case MAG_STEP4:  // N: A4 -B3- C4
		{            	 
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_B3, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);			 
			break;
		}
			
		case MAG_STEP5:     // N:A5 -B4 -C5
		{				 					
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_B4, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);	
			break;
		}
		case MAG_STEP6:  	// N:A6 -B5 -C6
		{                	
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_B5, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);				
			break;
		}
		case MAG_STEP7:
		{					// N:A7-C7
			MAG_value_off(MAG_A7, P_PIN_OFF);
			MAG_value_off(MAG_C7, P_PIN_OFF);			
			break;
		}
		default:
		{
		
			break;
		}
	}
}


void Vertical_select_n_on(void)
{

	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1 C1
		{			   		 //  
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_C1, N_PIN_ON);
			break;
		}
		case MAG_STEP2:  // N:  A2, B1, C2,
		{				
			MAG_value_out(MAG_A2, N_PIN_ON);
			MAG_value_out(MAG_B1, N_PIN_ON);
			MAG_value_out(MAG_C2, N_PIN_ON);
			break;
		}
	
		case MAG_STEP3:  // N: A3 -B2 - C3
		{		         
			MAG_value_out(MAG_A3, N_PIN_ON);
			MAG_value_out(MAG_B2, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);			
			break;       
		}
		case MAG_STEP4:  // N: A4 -B3- C4
		{            	 
			MAG_value_out(MAG_A4, N_PIN_ON);
			MAG_value_out(MAG_B3, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);			 
			break;
		}
			
		case MAG_STEP5:     // N:A5 -B4 -C5
		{				 					
			MAG_value_out(MAG_A5, N_PIN_ON);
			MAG_value_out(MAG_B4, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);	
			break;
		}
		case MAG_STEP6:  	// N:A6 -B5 -C6
		{                	
			MAG_value_out(MAG_A6, N_PIN_ON);
			MAG_value_out(MAG_B5, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);				
			break;
		}
		case MAG_STEP7:
		{					// N:A7-C7
			MAG_value_out(MAG_A7, N_PIN_ON);
			MAG_value_out(MAG_C7, N_PIN_ON);			
			break;
		}
		default:
		{
		
			break;
		}
	}
}


//=======================
//--------debug �汾������ͨʵ�� ��һ�е�ͨ    2023 01 16 

#if 0     // 1--3--5--7--2 --4--3

void Vertical_select_p_off(void)
{

	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1 C2
		{			   		 //  
			MAG_value_off(MAG_A1, P_PIN_OFF);
			MAG_value_off(MAG_C1, P_PIN_OFF);
			break;
		}
		case MAG_STEP2:  // N:  A2, B1, C2,
		{				
//			MAG_value_off(MAG_A2, P_PIN_OFF);
//			MAG_value_off(MAG_B1, P_PIN_OFF);
//			MAG_value_off(MAG_C2, P_PIN_OFF);
			
					 // N: A3 -B2 - C3
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_B2, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);	
			break;
		}
	
		case MAG_STEP3:  // N: A3 -B2 - C3
		{		         
//			MAG_value_off(MAG_A3, P_PIN_OFF);
//			MAG_value_off(MAG_B2, P_PIN_OFF);
//			MAG_value_off(MAG_C3, P_PIN_OFF);	
							// N:A5 -B4 -C5
			MAG_value_off(MAG_A5, P_PIN_OFF);
			MAG_value_off(MAG_B4, P_PIN_OFF);
			MAG_value_off(MAG_C5, P_PIN_OFF);	
			
			break;       
		}
		case MAG_STEP4:  // N: A4 -B3- C4
		{            	 
//			MAG_value_off(MAG_A4, P_PIN_OFF);
//			MAG_value_off(MAG_B3, P_PIN_OFF);
//			MAG_value_off(MAG_C4, P_PIN_OFF);		
						// N:A7-C7
			MAG_value_off(MAG_A7, P_PIN_OFF);
			MAG_value_off(MAG_C7, P_PIN_OFF);	
			
			break;
		}
			
		case MAG_STEP5:     // N:A5 -B4 -C5
		{				 					
//			MAG_value_off(MAG_A5, P_PIN_OFF);
//			MAG_value_off(MAG_B4, P_PIN_OFF);
//			MAG_value_off(MAG_C5, P_PIN_OFF);	
				
			
						// N:  A2, B1, C2,
			MAG_value_off(MAG_A3, P_PIN_OFF);
			MAG_value_off(MAG_B2, P_PIN_OFF);
			MAG_value_off(MAG_C3, P_PIN_OFF);	
			
			break;
		}
		case MAG_STEP6:  	// N:A6 -B5 -C6
		{                	
//			MAG_value_off(MAG_A6, P_PIN_OFF);
//			MAG_value_off(MAG_B5, P_PIN_OFF);
//			MAG_value_off(MAG_C6, P_PIN_OFF);	

							// N: A4 -B3- C4
			MAG_value_off(MAG_A4, P_PIN_OFF);
			MAG_value_off(MAG_B3, P_PIN_OFF);
			MAG_value_off(MAG_C4, P_PIN_OFF);	
			
			break;
		}
		case MAG_STEP7:
		{					// N:A7-C7
//			MAG_value_off(MAG_A7, P_PIN_OFF);
//			MAG_value_off(MAG_C7, P_PIN_OFF);		
		
				              // N:A6 -B5 -C6
			
			MAG_value_off(MAG_A6, P_PIN_OFF);
			MAG_value_off(MAG_B5, P_PIN_OFF);
			MAG_value_off(MAG_C6, P_PIN_OFF);
			
			
			break;
		}
		default:
		{
		
			break;
		}
	}
}

void Vertical_select_n_on(void)
{

	switch (sys_info.cur_mag_step)
	{
		case MAG_STEP1:     //  N: A1 C1
		{			   		 //  
			MAG_value_out(MAG_A1, N_PIN_ON);
			MAG_value_out(MAG_C1, N_PIN_ON);
			break;
		}
		case MAG_STEP2:  // N:  A2, B1, C2,
		{				
//			MAG_value_out(MAG_A2, N_PIN_ON);
//			MAG_value_out(MAG_B1, N_PIN_ON);
//			MAG_value_out(MAG_C2, N_PIN_ON);
			
						// N: A3 -B2 - C3
			MAG_value_out(MAG_A3, N_PIN_ON);
			MAG_value_out(MAG_B2, N_PIN_ON);
			MAG_value_out(MAG_C3, N_PIN_ON);	
			
			break;
		}
	
		case MAG_STEP3:  // N: A3 -B2 - C3
		{		         
//			MAG_value_out(MAG_A3, N_PIN_ON);
//			MAG_value_out(MAG_B2, N_PIN_ON);
//			MAG_value_out(MAG_C3, N_PIN_ON);	

				// N:A5 -B4 -C5
			MAG_value_out(MAG_A5, N_PIN_ON);
			MAG_value_out(MAG_B4, N_PIN_ON);
			MAG_value_out(MAG_C5, N_PIN_ON);	
			
			break;       
		}
		case MAG_STEP4:  // N: A4 -B3- C4
		{            	 
//			MAG_value_out(MAG_A4, N_PIN_ON);
//			MAG_value_out(MAG_B3, N_PIN_ON);
//			MAG_value_out(MAG_C4, N_PIN_ON);	

							// N:A7-C7
			MAG_value_out(MAG_A7, N_PIN_ON);
			MAG_value_out(MAG_C7, N_PIN_ON);	
			break;
		}
			
		case MAG_STEP5:     // N:A5 -B4 -C5
		{				 					
//			MAG_value_out(MAG_A5, N_PIN_ON);
//			MAG_value_out(MAG_B4, N_PIN_ON);
//			MAG_value_out(MAG_C5, N_PIN_ON);	

			                    // N:  A2, B1, C2,
			MAG_value_out(MAG_A2, N_PIN_ON);
			MAG_value_out(MAG_B1, N_PIN_ON);
			MAG_value_out(MAG_C2, N_PIN_ON);
			break;
		}
		case MAG_STEP6:  	// N:A6 -B5 -C6
		{                	
//			MAG_value_out(MAG_A6, N_PIN_ON);
//			MAG_value_out(MAG_B5, N_PIN_ON);
//			MAG_value_out(MAG_C6, N_PIN_ON);

							 // N: A4 -B3- C4
			MAG_value_out(MAG_A4, N_PIN_ON);
			MAG_value_out(MAG_B3, N_PIN_ON);
			MAG_value_out(MAG_C4, N_PIN_ON);	
			
			break;
		}
		case MAG_STEP7:
		{					// N:A7-C7
//			MAG_value_out(MAG_A7, N_PIN_ON);
//			MAG_value_out(MAG_C7, N_PIN_ON);	

									// N:A6 -B5 -C6
			MAG_value_out(MAG_A6, N_PIN_ON);
			MAG_value_out(MAG_B5, N_PIN_ON);
			MAG_value_out(MAG_C6, N_PIN_ON);	
			
			
			break;
		}
		default:
		{
		
			break;
		}
	}
}

#endif
//----------------------------


void Vertical_magnetic_out(void)
{
	all_p_on_n_off();
	Vertical_select_p_off();
	delay_1ms_timer_start();
}


void Magen_select_n_pins_on(void)
{

	switch (sys_info.p_magn_data->mag_type)
	{
		case SLOPE_TYPE1:
		{
			Slope1_select_n_on();
			break;
		}
	
		case SLOPE_TYPE2:
		{
			Slope2_select_n_on();
			break;
		}
		default:
		{
			Vertical_select_n_on();
			break;
		}
	}

}






