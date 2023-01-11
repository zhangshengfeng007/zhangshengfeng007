#ifndef __UART_SEND_H
#define __UART_SEND_H
#define buffsize 255//8
extern uint8_t recv_data_buff[buffsize];
extern void Uart2_Send_Data(void);
extern void User_Uart_Init(void);
extern void Aging_Test(void);
extern void Sys_Command_Handle(uint8_t *datacom);

//==========================Manufacturer Data==================================//
//ID_code---------------------
#define AMIRO_ID  0x0001
#define XIAOMI_ID 0x0002
//MODEL_code-----------------
#define AML_Series 0x0001
#define ABF_Series 0x0002
//Device ID-------------------
#define R1_PRO_LITE     0x0010
#define S1              0x0001
#define R1_PRO_MAX      0x0011
#define O2_OVERSEAS_VERSION 0x0001
//Soft_Version----------------
#define SOFT_VER        0x23
//Hard_Version----------------
#define HARDWARE_VER    0x60
//Direction ID----------------
#define Direction_ID_Recv 0x00 //pc->��Ʒ
#define Direction_ID_Send 0x10 //��Ʒ->pc
//Soft_Version 

#define AUTO_TEST_ON		0
#define AUTO_TEST_OFF		0xffff

//command---------------------
#define Comm_Inset_Testmode					0x0001	//�������ģʽ
#define Comm_Manufactur_Info				0x0002	//��ȡ������Ϣ���汾��
#define Comm_Ntc_Temp								0x0003	//��ȡNTC�¶�ֵ
#define Comm_Batter_Value						0x0004	//��ȡ��ص�ѹֵ
#define Comm_Moto_Test							0x0005	//���������
#define Comm_LED_Test								0x0006	//�򿪵ư壨LED-IR-ON�����
#define Comm_Electrode_Test					0x0007	//�缫ͷ��ͨ�Բ���
#define Comm_RF_Test								0x0008	//RF���ģʽ
#define Comm_EMS_Test								0x0009	//EMS���ģʽ
#define Comm_KEY_Test								0x000A	//��������
#define Comm_All_Light							0x000B	//���еƹ����
#define Comm_G_Sensor_Test					0x000C	//G-SENSOR����
#define Comm_RGB_Sensor_Calib				0x000D	//RGB-SENSORУ׼
#define Comm_RGB_Sensor_Test				0x000E	//RGB-SENSOR����
#define Comm_Aging_Model						0x000F	//�ϻ�ģʽ����ߵ�λ��������û�磩
#define Comm_Life_Test							0x0010	//�����������ԣ������ߵ�λ�����
#define Comm_Inset_Bootloader				0x0011	//����BootLoader������BootLoader������
#define Comm_Check_Version					0x0012	//�汾У�飨���������������У��汾�ţ�
#define Comm_Inset_Standby					0x0013	//����ģʽ�����Դ������� 
#define Comm_Quit_AUTOTest					0x0014	//�˳�����ģʽ
//#define Comm_Quit_Standby				0x000E	//�˳�����ģʽ 

//--------------------------------
#endif




