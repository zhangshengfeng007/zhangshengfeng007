/**
******************************************************************************
* @file    ZJ_LTR831.c
* @author  L
* @version V1.0
* @date    2021.05.27
* @brief   LTR831
* @check   2021.05.27
* @history 
******************************************************************************
*/
#include "includes.h"




extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;
const uint32_t I2c2Timeout = EVAL_I2Cx_TIMEOUT_MAX;
uint8_t I2c1_Buf_Read[10]={0};
uint8_t LTR831_GetRawData(_COLOR_RGBC_Typedef *rgbc);

_COLOR_RGBC_Typedef 		RGB_Handle;
_White_Balance_Typedef  White_Balance;
_Calibraton_Key  				Cal_Key;

extern void MX_I2C2_Init(void);
void LTR831_INIT(void);
uint8_t LTR831_Read_Byte(uint8_t Reg);

#ifdef COLOR_SENSOR_ENABLE
//--------------------------------------------------------------------------------//
void RGB_SENSER_RESET(void)
{
  SENSOR_PWR_SW_OFF;            //close
  HAL_Delay(2);
  SENSOR_PWR_SW_ON;             //open
  HAL_Delay(2);
  LTR831_INIT();
}
//--------------------------------------------------------------------------------//
_init_result_e CHECK_RGB_IIC(void)
{
  if(LTR831_Read_Byte(LTR381_PART_ID) == 0xC2)
  {
    Sys_Info.RGB_sensor_state = INIT_SUCCESS;
    return INIT_SUCCESS;
  }
  else
  {
    Sys_Info.RGB_sensor_state = INIT_ERROR;
    return INIT_ERROR;
  }
}
//--------------------------------------------------------------------------------//
static void I2C_LTR831_Error (void)
{
  HAL_I2C_DeInit(&hi2c2);
  HAL_Delay(1);
  MX_I2C2_Init();
}
//--------------------------------------------------------------------------------//
HAL_StatusTypeDef LTR831_Write_Byte(uint8_t Reg, uint8_t data)
{
  if(HAL_I2C_Mem_Write(&hi2c2, LTR831_W_ADDRESS, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2c2Timeout)!= HAL_OK)
  {
    I2C_LTR831_Error();
  }        
  return HAL_OK;
}
//--------------------------------------------------------------------------------//
uint8_t LTR831_Read_Byte(uint8_t Reg)
{
  uint8_t data;
  
  if(HAL_I2C_Mem_Read(&hi2c2, LTR831_R_ADDRESS, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &data, 1, I2c2Timeout) != HAL_OK)
  {
    I2C_LTR831_Error();
  }        
  return data;
}
//--------------------------------------------------------------------------------//
void LTR831_INIT(void)
{
  if(CHECK_RGB_IIC() == INIT_SUCCESS)
  {
    //LTR831_Write_Byte(LTR381_MAIN_CTRL,CTRL_SW_RESET);        //reset
    //HAL_Delay(10);
    LTR831_Write_Byte(LTR381_ALS_CS_MEAS_RATE,ALS_CS_RES_16BIT | ALS_CS_MEAS_25MS); //16bit  25ms
    HAL_Delay(1);
    LTR831_Write_Byte(LTR381_ALS_CS_GAIN,MODE_ALS_Range3);      //Gain Range 3
    HAL_Delay(1);
    LTR831_Write_Byte(LTR381_MAIN_CTRL,CTRL_CS_MODE_CS | CTRL_ALS_CS_EN); //CS mode ALS active
    
    if(LTR831_Read_Byte(LTR381_ALS_CS_MEAS_RATE) != (ALS_CS_RES_16BIT | ALS_CS_MEAS_25MS))
    {
      Sys_Info.RGB_sensor_state = INIT_ERROR;
    }
    else
    {
      Sys_Info.RGB_sensor_state = INIT_SUCCESS;
    }
  }
}
//--------------------------------------------------------------------------------//
uint32_t  MAX(uint32_t R,uint32_t G,uint32_t B)
{
  if((R>G)&&(R>B))
  {
    return R;
  }
  else if((G>R)&&(G>B))
  {
    return G;
  }
  else 
  {
    return B;
  }
}
/*******************************************************************************
* @brief LTR831获取各个通道数据
*
* @return 0 - 转换完成，数据可用
*   	     1 - 转换未完成，数据不可用
*******************************************************************************/
uint8_t LTR831_GetRawData(_COLOR_RGBC_Typedef *rgbc)
{
  uint8_t Data0,Data1,Data2;
  
  if(CHECK_RGB_IIC() == INIT_SUCCESS)
  {
    //寄存器值----------------------------------------------------------------/
    Data0 = LTR831_Read_Byte(LTR381_DATA_GREEN_0);  //Green
    Data1 = LTR831_Read_Byte(LTR381_DATA_GREEN_1);
    Data2 = LTR831_Read_Byte(LTR381_DATA_GREEN_2);
    rgbc->g =(Data2<<16)| (Data1 << 8) | Data0;
    Data0 = LTR831_Read_Byte(LTR381_DATA_BLUE_0);   //Blue
    Data1 = LTR831_Read_Byte(LTR381_DATA_BLUE_1);
    Data2 = LTR831_Read_Byte(LTR381_DATA_BLUE_2);
    rgbc->b =(Data2<<16)| (Data1 << 8) | Data0;
    Data0 = LTR831_Read_Byte(LTR381_DATA_RED_0);    //Red
    Data1 = LTR831_Read_Byte(LTR381_DATA_RED_1);
    Data2 = LTR831_Read_Byte(LTR381_DATA_RED_2);
    rgbc->r =(Data2<<16)| (Data1 << 8) | Data0;	
    Data0 = LTR831_Read_Byte(LTR381_DATA_IR_0);     //IR
    Data1 = LTR831_Read_Byte(LTR381_DATA_IR_1);
    Data2 = LTR831_Read_Byte(LTR381_DATA_IR_2);
    rgbc->IR =(Data2<<16)| (Data1 << 8) | Data0;
  }
  //比例因子----------------------------------------------------------------/
  rgbc->R_Factor = (float)(rgbc->g)/(float)(rgbc->r);																		//R比例因子计算，参考值G
  rgbc->B_Factor = (float)(rgbc->g)/(float)(rgbc->b);																		//B比例因子计算，参考值G
  //RGB通道校准值-----------------------------------------------------------/
  rgbc->RGB_R =(uint32_t) (rgbc->r * White_Balance.Calibration_R);
  rgbc->RGB_G =(uint32_t) (rgbc->g * White_Balance.Calibration_G);
  rgbc->RGB_B =(uint32_t) (rgbc->b * White_Balance.Calibration_B);
  //计算AVG和PEAK值---------------------------------------------------------/
  //rgbc->RGB_Average = (rgbc->RGB_R+rgbc->RGB_G+rgbc->RGB_B)/3;        //Avg值	
  rgbc->RGB_Peak    = MAX(rgbc->RGB_R,rgbc->RGB_G,rgbc->RGB_B);         //Peak值
  //色温和照度值计算--------------------------------------------------------/
  //rgbc->Cct       = 3567 * rgbc->RGB_B / rgbc->RGB_R + 1962;          //色温值
#if 1
  rgbc->Lux_Calc  = (uint32_t) (((0.74 * rgbc->g)/(3*0.25))*(1-(0.033*(rgbc->IR/rgbc->g))) * WFAC ); //照度值
#else
  rgbc->Lux_Calc  = (uint32_t) (rgbc->g * 74/100/3*4 * (1- rgbc->IR*33/1000/rgbc->g) * WFAC) ;
#endif
  rgbc->RGB_Lux = (uint32_t) (rgbc->Lux_Calc * White_Balance.Calibration_Lux);
  //RGB比例值---------------------------------------------------------------/
  rgbc->R_SCALED    = (uint16_t) (((float)rgbc->RGB_R / (float)rgbc->RGB_Peak)*255);
  rgbc->G_SCALED    = (uint16_t) (((float)rgbc->RGB_G / (float)rgbc->RGB_Peak)*255);
  rgbc->B_SCALED    = (uint16_t) (((float)rgbc->RGB_B / (float)rgbc->RGB_Peak)*255);
  rgbc->Lux_SCALED  = (uint16_t) (((float)rgbc->RGB_Lux /(float)65535)	* 1000);
  
  return 0;
}
//-------------------------------------------------------------------------------------//	
//颜色传感器读取错误判断
//-------------------------------------------------------------------------------------//
uint8_t colour_sensor_correctness(_COLOR_RGBC_Typedef *RGB_PRT)
{
  if((RGB_PRT->r > 200000)||(RGB_PRT->g > 200000)||(RGB_PRT->b > 200000))
  {
    return 1;
  }
//  if(RGB_PRT->r == RGB_PRT->b)
//  {
//    if(RGB_PRT->b == RGB_PRT->g)
//    {
//      return 1;
//    }
//  }
  return 0;
}
//-------------------------------------------------------------------------------------//	
///颜色识别判断不同颜色  返回不同颜色  
//-------------------------------------------------------------------------------------//
uint8_t ColorRecognition(void)
{
  static uint8_t lase_choice = COLOUR_OTHER;
  
  _COLOUR_Enum_Typedef  color = COLOUR_OTHER;
  
  if(Sys_Info.Key_State.Mode_choice_key != lase_choice)
  {
    color = COLOUR_OTHER;	
  }
  lase_choice = Sys_Info.Key_State.Mode_choice_key;
  
  if(color == COLOUR_OTHER)
  {
    LTR831_GetRawData(&RGB_Handle);
    
    if(((RGB_Handle.R_SCALED >= RGB_ORANGE_R_MIN) && (RGB_Handle.R_SCALED <= RGB_ORANGE_R_MAX)) &&\
       ((RGB_Handle.G_SCALED >= RGB_ORANGE_G_MIN) && (RGB_Handle.G_SCALED <= RGB_ORANGE_G_MAX)) &&\
       ((RGB_Handle.B_SCALED >= RGB_ORANGE_B_MIN) && (RGB_Handle.B_SCALED <= RGB_ORANGE_B_MAX)))
    {
      if((RGB_Handle.Lux_SCALED >= ORANGE_Lux_Calc_Min) && (RGB_Handle.Lux_SCALED <= ORANGE_Lux_Calc_Max))
      {
        color = COLOUR_ORANGE;
      }
    }
    else if(((RGB_Handle.R_SCALED >= RGB_BLUE_R_MIN) && (RGB_Handle.R_SCALED <= RGB_BLUE_R_MAX)) &&\
            ((RGB_Handle.G_SCALED >= RGB_BLUE_G_MIN) && (RGB_Handle.G_SCALED <= RGB_BLUE_G_MAX)) &&\
            ((RGB_Handle.B_SCALED >= RGB_BLUE_B_MIN) && (RGB_Handle.B_SCALED <= RGB_BLUE_B_MAX)))
    {
      if((RGB_Handle.Lux_SCALED >= BLUE_Lux_Calc_Min) && (RGB_Handle.Lux_SCALED <= BLUE_Lux_Calc_Max))
      {
        color = COLOUR_BLUE;
      }
    }
    else if(((RGB_Handle.R_SCALED >= RGB_GREEN_R_MIN) && (RGB_Handle.R_SCALED <= RGB_GREEN_R_MAX)) &&\
            ((RGB_Handle.G_SCALED >= RGB_GREEN_G_MIN) && (RGB_Handle.G_SCALED <= RGB_GREEN_G_MAX)) &&\
            ((RGB_Handle.B_SCALED >= RGB_GREEN_B_MIN) && (RGB_Handle.B_SCALED <= RGB_GREEN_B_MAX)))
    {
      if((RGB_Handle.Lux_SCALED >= GREEN_Lux_Calc_Min) && (RGB_Handle.Lux_SCALED <= GREEN_Lux_Calc_Max))
      {
        color = COLOUR_GREEN;
      }
    }
    else if(((RGB_Handle.R_SCALED >= RGB_BROWN_R_MIN) && (RGB_Handle.R_SCALED <= RGB_BROWN_R_MAX)) &&\
            ((RGB_Handle.G_SCALED >= RGB_BROWN_G_MIN) && (RGB_Handle.G_SCALED <= RGB_BROWN_G_MAX)) &&\
            ((RGB_Handle.B_SCALED >= RGB_BROWN_B_MIN) && (RGB_Handle.B_SCALED <= RGB_BROWN_B_MAX)))
    {
      if((RGB_Handle.Lux_SCALED >= BROWN_Lux_Calc_Min) && (RGB_Handle.Lux_SCALED <= BROWN_Lux_Calc_Max))
      {
				if(RGB_Handle.R_Factor < RGB_Handle.B_Factor)//((RGB_Handle.R_Factor-RGB_Handle.B_Factor)> 0.4)
        color = COLOUR_BROWN;
      }
    }
    else
    {
      color = COLOUR_OTHER;
			
    }
  }
  
  if(colour_sensor_correctness(&RGB_Handle))  //颜色传感器错误判断
  {
    color = READ_ERR;
  }  
  
  return color;
}

/**************************************************************************************
* FunctionName   :  RGB_Sensor_Calibration
* Description    :  RGB传感器校准动作 -
* EntryParameter :  进入方式，按下power键，2秒内连按gear键5次，进入此模式后，按power键校准色卡，按Gear键读取色卡
* Description    : --
* ReturnValue    :
**************************************************************************************/
void RGB_Sensor_Calibration(void)
{
  //SYS_BUS_SW_UP();               //开启系统开关
  //LOAD_BUS_SW_UP();              //开启负载开关
  
  SYS_BUS_SW_ON;
  LOAD_BUS_SW_ON;
  BACKLIGHT_OFF;
  RGB_BACKLIGHT_ON;        //开启颜色识别传感器背光
  SENSOR_PWR_SW_ON;        //颜色传感器上电
  HAL_Delay(1);
  LTR831_INIT();
  
  Cal_Key.timer1_cnt = HAL_GetTick();
  
  while(1)
  {
#ifdef IWDG_ENABLE
    HAL_IWDG_Refresh(&hiwdg);//喂狗 
#endif
    if(HAL_GetTick() - Cal_Key.timer1_cnt >= 100)
    {
      Cal_Key.timer1_cnt = HAL_GetTick();
      
      if(Power_key_Read_In == GPIO_PIN_RESET)//电源键按下进行一次校准动作并写入FLASH
      {
        if(!Cal_Key.set_power_key_flag)
        {
          Cal_Key.set_power_key_flag = 1;
          Cal_Key.Calibraton_R_Avg = 0;
          Cal_Key.Calibraton_B_Avg = 0;
          Cal_Key.Calibraton_Lux_Avg = 0;
          Cal_Key.Calibraton_Start_flag = 1;
          Cal_Key.timer2_cnt = HAL_GetTick();
          Set_Color_Handle(0,0,0,0);
          Set_Color_Handle(0,0,0,1);
          LCD_output_transf(Led_Colour);
        }					
      }
      else
      {
        Cal_Key.set_power_key_flag = 0;
      }
      //------------------------------------------------------------------------------//
      if(Gear_key_Read_In == GPIO_PIN_RESET) //在RGB灯上显示校准值后的当前颜色
      {
        if(!Cal_Key.set_gears_key_flag)
        {
          Cal_Key.set_gears_key_flag = 1;
          Cal_Key.timer2_cnt = HAL_GetTick();
          LTR831_GetRawData(&RGB_Handle);
          Set_Color_Handle(RGB_Handle.R_SCALED,RGB_Handle.G_SCALED,RGB_Handle.B_SCALED,1);
          LCD_output_transf(Led_Colour);
        }
      }
      else
      {
        Cal_Key.set_gears_key_flag = 0;
      }
      //------------------------------------------------------------------------------//
      if(Cal_Key.Calibraton_Start_flag) //取5次平均值写入flash
      {
        if(++Cal_Key.Calibraton_Start_Cnt <= 5)
        {
          LTR831_GetRawData(&RGB_Handle);
          Cal_Key.Calibraton_R_Avg += RGB_Handle.R_Factor;
          Cal_Key.Calibraton_B_Avg += RGB_Handle.B_Factor;
          Cal_Key.Calibraton_Lux_Avg += RGB_Handle.Lux_Calc;
        }
        else
        {
          Cal_Key.Calibraton_Start_flag  = 0;
          Cal_Key.Calibraton_Start_Cnt = 0;
          Cal_Key.Calibraton_R_Avg = Cal_Key.Calibraton_R_Avg /5;
          Cal_Key.Calibraton_B_Avg = Cal_Key.Calibraton_B_Avg /5;
          Cal_Key.Calibraton_Lux_Avg = 10000/(Cal_Key.Calibraton_Lux_Avg /5);//10000
          Flash_Data_Save_RGB_FACTOR(Cal_Key.Calibraton_R_Avg,1,Cal_Key.Calibraton_B_Avg,Cal_Key.Calibraton_Lux_Avg);
          
          White_Balance.Calibration_R = Cal_Key.Calibraton_R_Avg;
          White_Balance.Calibration_G = 1;
          White_Balance.Calibration_B = Cal_Key.Calibraton_B_Avg;
          White_Balance.Calibration_Lux = Cal_Key.Calibraton_Lux_Avg;
          
          Set_Color_Handle(255,255,255,0);
          Set_Color_Handle(RGB_Handle.R_SCALED,RGB_Handle.G_SCALED,RGB_Handle.B_SCALED,1);
          LCD_output_transf(Led_Colour);
        }
      }
    }
    //------------------------------------------------------------------------------//
    if((HAL_GetTick()-Cal_Key.timer2_cnt > 10000)&&(!Cal_Key.Calibraton_Start_flag))//10S后退出校准模式，复位系统
    {
      HAL_NVIC_SystemReset();		
    }
  }
}

#endif



