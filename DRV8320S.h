
#ifndef __DRV8320S_H
#define __DRV8320S_H

//#include "stm32g4xx_hal.h"
#include "stdint.h"
#include "BSP_DRV8320_STM32/BSP_DRV8320.h"

#define DRV8320_ADDR_MASK 0x7800
#define DRV8320_DATA_MASK 0x07FF
#define DRV8320_RW_MASK   0x8000
#define DRV_OFFSET 11//сдвиг влево адреса регистра , к которому идет запись

//DRV8320S SPI Register Map
#define DRV8320S_FAULT_STATUS_1 		0x00<<DRV_OFFSET
#define DRV8320S_VGS_STATUS_1 			0x01<<DRV_OFFSET
#define DRV8320S_DRV_CTRL_1 				0x02<<DRV_OFFSET
#define DRV8320S_GATE_DRV_HS_1 			0x03<<DRV_OFFSET
#define DRV8320S_GATE_DRV_LS_1 			0x04<<DRV_OFFSET
#define DRV8320S_OCP_CTRL_1 				0x05<<DRV_OFFSET


#define GATE_P_OFFSET			 4
#define GATE_LOCK_OFFSET 	 8
//this struct will be used to activate and to switch between different registers to analyze



typedef enum{
	Read_free=1u,
	Fault_read_active=1u<<1,
	Vgs_read_active=1u<<2
}ReadStatus;

typedef enum{
	FaultNone=0,		//система работает как обычно	
	FaultDetected=1,//прервать работу до выяснения
	FaultProcessed=2//выяснения ошибки
}FaultStatus;

typedef struct{
	uint16_t FAULT_Reg;
	uint16_t VGS_Reg;
	uint16_t DRV_CTRL_Reg;
	uint16_t GDHS_Reg;
	uint16_t GDLS_Reg;
	uint16_t OCP_Control_Reg;
}DRV8320SReg_Struct;

typedef struct{
	ReadStatus RdStatus;
	DRV8320SReg_Struct RegStruct;
	uint16_t Rx_data;//its just one half word of data to store a read of status registers
	const DRV8320Functions * const funcList;
}DRV_Struct;

void DRV8320S_Init(void);
void DRV8320S_LockConfig(uint8_t state);
void DRV8320S_clearFault(void);
void DRV8320S_SetENABLE(uint8_t state);
void DRV8320S_Write_reg(uint16_t addr,uint16_t reg);
uint16_t DRV8320S_Read_reg(uint16_t addr);
void DRV8320S_GetFaultStatus(void);
void DRV8320S_GetVgsStatus(void);
uint8_t DRV8320_CheckRxStatus(void);
void DRV8320S_ReceiveCheck(void);
void DRV8320S_GetStatus(void);

//another approach for bits 
typedef enum{
	VDS_LC=1u,
	VDS_HC=1u<<1,
	VDS_LB=1u<<2,
	VDS_HB=1u<<3,
	VDS_LA=1u<<4,
	VDS_HA=1u<<5,
	OTSD=1u<<6,
	UVLO=1u<<7,
	GDF=1u<<8,
	VDS_OCP=1u<<9,
	FAULT=1u<<10
}FaultBits;

typedef enum{
	VGS_LC=1u,
	VGS_HC=1u<<1,
	VGS_LB=1u<<2,
	VGS_HB=1u<<3,
	VGS_LA=1u<<4,
	VGS_HA=1u<<5,
	CPUV=1u<<6,
	OTW=1u<<7,
	//SC_OC=1u<<8,
	//SB_OC=1u<<9,
	//SA_OC=1u<<10
}VgsBits;

typedef enum{
	CLR_FLT=1u,
	BRAKE=1u<<1,
	COAST=1u<<2,
	PWM_DIR=1u<<3,
	PWM_COM=1u<<4,
	PWM_MODE_0=1u<<5,
	PWM_MODE_1=1u<<6,
	OTW_REP=1u<<7,
	DIS_GDF=1u<<8,
	DIS_CPUV=1u<<9
}DrvCtrlBits;

typedef enum{
	OCP_DEG_0=1u<<4,
	OCP_DEG_1=1u<<5,
	MODE_0=1u<<6,
	MODE_1=1u<<7,
	DT_0=1u<<8,
	DT_1=1u<<9,
	TRETRY=1u<<10
}OcpCtrlBits;


#endif
