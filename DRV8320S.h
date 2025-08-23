
#ifndef __DRV8320S_H
#define __DRV8320S_H

#include "stdint.h"
#include "BSP_DRV8320.h"

#define DRV8320_ADDR_MASK   0x7800
#define DRV8320_DATA_MASK   0x07FF
#define DRV8320_RW_MASK     0x8000
#define DRV_OFFSET          11//offset of address in register map

//DRV8320S SPI Register Map
#define DRV8320S_FAULT_STATUS_1     0x00<<DRV_OFFSET
#define DRV8320S_VGS_STATUS_1       0x01<<DRV_OFFSET
#define DRV8320S_DRV_CTRL_1         0x02<<DRV_OFFSET
#define DRV8320S_GATE_DRV_HS_1      0x03<<DRV_OFFSET
#define DRV8320S_GATE_DRV_LS_1      0x04<<DRV_OFFSET
#define DRV8320S_OCP_CTRL_1         0x05<<DRV_OFFSET


#define GATE_P_OFFSET       4
#define GATE_LOCK_OFFSET    8
//this struct will be used to activate and to switch between different registers to analyze

/* fault status bitfields */
#define VDS_LC      0x0001
#define VDS_HC      0x0002
#define VDS_LB      0x0004
#define VDS_HB      0x0008
#define VDS_LA      0x0010
#define VDS_HA      0x0020
#define OTSD        0x0040
#define UVLO        0x0080
#define GDF         0x0100
#define VDS_OCP     0x0200
#define FAULT       0x0400

/* VGS reg bitfields */
#define VGS_LC      0x0001
#define VGS_HC      0x0002
#define VGS_LB      0x0004
#define VGS_HB      0x0008
#define VGS_LA      0x0010
#define VGS_HA      0x0020
#define CPUV        0x0040
#define OTW         0x0080

/* DRV_CTRL reg bitfields */
#define CLR_FLT     0x0001
#define BRAKE       0x0002
#define COAST       0x0004
#define PWM_DIR     0x0008
#define PWM_COM     0x0010
#define PWM_MODE_0  0x0020
#define PWM_MODE_1  0x0040
#define OTW_REP     0x0080
#define DIS_GDF     0x0100
#define DIS_CPUV    0x0200

/* OCP reg bitfields */
#define OCP_DEG_0   0x0010
#define OCP_DEG_1   0x0020
#define MODE_0      0x0040
#define MODE_1      0x0080
#define DT_0        0x0100
#define DT_1        0x0200
#define TRETRY      0x0400



/** @brief struct with function for processing faults */
typedef struct {
  void (*OTSD_FaultProcess)(void);
  void (*UVLO_FaultProcess)(void);
  void (*GDF_FaultProcess)(void);
  void (*VDS_OCP_FaultProcess)(void);
  void (*CPUV_FaultProcess)(void);
  void (*OTW_FaultProcess)(void);
}FaultStatusFunctions;


typedef enum{
  Read_free=1u,
  Fault_read_active=1u<<1,
  Vgs_read_active=1u<<2
}ReadStatus;

typedef enum{
  FaultNone=0,		
  FaultDetected=1,
  FaultProcessed=2
}FaultStatus;

/** @brief: drv8320 register struct */
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
  const FaultStatusFunctions * const faultFuncList;
}DRV_Struct;

/** @brief initialization of peripheral and drv8320s module */
void DRV8320S_Init(void);

/** @brief function to lock config of drive registers 
 *  @param state 1 or 0 to lock and unlock config 
*/
void DRV8320S_LockConfig(uint8_t state);

/** @brief function to clear fault in status register */
void DRV8320S_clearFault(void);

/** @brief function to enable drv8320
 *  @param state 1 or 0 to enable or disable drv8320
 */
void DRV8320S_SetENABLE(uint8_t state);


void DRV8320S_ReceiveCheck(void);


void DRV8320S_GetStatus(void);


void DRV8320_FaultCallback(void);




#endif
