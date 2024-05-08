#ifndef _CHARGE_H
#define _CHARGE_H

#include "system.h"

#define		RELEASE_400V_EN()			RELEASE_400V=1
#define		RELEASE_400V_DIS()			RELEASE_400V=0

#define		MINSUPPLYVOLTAGE		101
#define		MAXSUPPLYVOLTAGE		140

typedef struct xChargeStatus_t_
{
	uint8_t  b_charge_busy;
	//uint8_t uc_lastvaluetesttimes;
	uint8_t uc_lastvaluetestcnt;
	uint8_t uc_errortimes;
	//uint8_t uc_supplyerrortimes;
	uint16_t uc_curpwmduty;
	uint16_t ul_current_duty;
	uint16_t ul_chargevalue;		// adc大电容电压
	uint32_t ul_chargeBuff;		// 充电400V检测12个值累加值
}xChargeStatus_t;

extern xChargeStatus_t sx_ChargeStatus;
extern uint8_t vrefunstableflag;

#define		b_ChargeBusy_Set(x)		sx_ChargeStatus.b_charge_busy=x
#define		b_ChargeBusy_Get()		sx_ChargeStatus.b_charge_busy

void Charge_Init(void);
void Charge_Handler(void);
void VDD_Test_Handler(void);
void Charge_Duty_Set(uint8_t num);
void ChargeInit_Cb(void);
void SupplyVref_Test(void);


#endif
 