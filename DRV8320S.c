#include "DRV8320S.h"
#include "main.h"
#include "stm32g4xx_hal.h"
#include "spi.h"


/** @brief: precalculated values for current config of gate drivers*/
#define IDRIVEN	0xA //>0.659A
#define IDRIVEP	0xD //>0.527A

/** @brief: a struct to save data of driver*/
DRV_Struct drv_struct;

/** @brief: function to set chip select*/
static void DRV8320S_SetCS(void){
	GPIOB->BSRR=DRV_CS_Pin<<16;
}

/** @brief: function to reset chip select*/
static void DRV8320S_ResetCS(void){
	GPIOB->BSRR=DRV_CS_Pin;
}

/** @brief: initialization of regs */
void DRV8320S_Init(void){
	SPI1->CR2|=SPI_CR2_DS;
	SPI1->CR1|=SPI_CR1_MSTR|SPI_CR1_SPE;
	//while(!(FAULT_GPIO_Port->IDR&FAULT_Pin));
	drv_struct.RegStruct.DRV_CTRL_Reg=0;
	DRV8320S_Write_reg(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
	
	
	DRV8320S_LockConfig(0);
	drv_struct.RegStruct.GDHS_Reg|=(IDRIVEP<<GATE_P_OFFSET)|IDRIVEN;
	DRV8320S_Write_reg(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
	
	drv_struct.RegStruct.GDLS_Reg|=(3<<GATE_LOCK_OFFSET)|(IDRIVEP<<GATE_P_OFFSET)|IDRIVEN;
	DRV8320S_Write_reg(DRV8320S_GATE_DRV_LS_1,drv_struct.RegStruct.GDLS_Reg);
	
	DRV8320S_LockConfig(1);
	
	//ocp mode 00b (vds latching)
	drv_struct.RegStruct.OCP_Control_Reg=DT_1|DT_0;
	DRV8320S_Write_reg(DRV8320S_OCP_CTRL_1,drv_struct.RegStruct.OCP_Control_Reg);
}

/** @brief: write reg */
void DRV8320S_Write_reg(uint16_t addr,uint16_t reg){
    uint16_t data = (addr)|(reg&DRV8320_DATA_MASK);
		DRV8320S_SetCS();
		SPI1->DR=data;
		while(!(SPI1->SR&SPI_SR_TXE));
		while(SPI1->SR&SPI_SR_BSY);
		if(SPI1->SR&SPI_SR_RXNE){
			uint16_t dummy=SPI1->DR;
		}
		DRV8320S_ResetCS();
}

/** @brief: read reg */
uint16_t DRV8320S_Read_reg(uint16_t addr){
		if(SPI1->SR&SPI_SR_RXNE){
			uint16_t dummy=SPI1->DR;
		}
		uint16_t data= (addr)|0x8000;
		DRV8320S_SetCS();
		SPI1->DR=data;
		while(!(SPI1->SR&SPI_SR_RXNE));
		while(SPI1->SR&SPI_SR_BSY);
		DRV8320S_ResetCS();
    return SPI1->DR;
}

/** @brief:get status from both status registers*/
void DRV8320S_GetStatus(void){
	drv_struct.RegStruct.FAULT_Reg=DRV8320S_Read_reg(DRV8320S_FAULT_STATUS_1);
	drv_struct.RegStruct.VGS_Reg=DRV8320S_Read_reg(DRV8320S_VGS_STATUS_1);
}

/** @brief: clearing fault of registers */
void DRV8320S_clearFault(void){
	DRV8320S_Write_reg(DRV8320S_DRV_CTRL_1,drv_struct.RegStruct.DRV_CTRL_Reg&(~CLR_FLT));
}

/** @brief:locking a configuration of a driver registers */
void DRV8320S_LockConfig(uint8_t state){
	if(state){
		drv_struct.RegStruct.GDHS_Reg|=(6<<GATE_LOCK_OFFSET);
	}
	else{
		drv_struct.RegStruct.GDHS_Reg|=(3<<GATE_LOCK_OFFSET);
	}
	DRV8320S_Write_reg(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
}

/** @brief: set reset enable */
void DRV8320S_SetENABLE(uint8_t state){
	if(state){
		DRV_EN_GPIO_Port->BSRR=DRV_EN_Pin;
	}
	else{
		DRV_EN_GPIO_Port->BSRR=DRV_EN_Pin<<16;
	}
}

/** @brief:that function is only to analyze content of received fault register */
void DRV8320S_ReceiveCheck(void){
    if(drv_struct.RdStatus==Fault_read_active){//a fault register may activate its read with fault pin too 
      if(drv_struct.Rx_data&FAULT){  

        if(drv_struct.Rx_data&OTSD){
            //do something
        }
        if(drv_struct.Rx_data&UVLO){
            //do something
        }
        if(drv_struct.Rx_data&GDF){
            //do something
        }
        if(drv_struct.Rx_data&VDS_OCP){
					if(drv_struct.Rx_data&VDS_LC){
							//do something
					}
					if(drv_struct.Rx_data&VDS_HC){
							//do something
					}
					if(drv_struct.Rx_data&VDS_LB){
							//do something
					}
					if(drv_struct.Rx_data&VDS_HB){
							//do something
					}
					if(drv_struct.Rx_data&VDS_LA){
							//do something
					}
					if(drv_struct.Rx_data&VDS_HA){
						 //do something
					}
        }

      }
      drv_struct.RdStatus=Read_free;
    }
    if(drv_struct.RdStatus==Vgs_read_active){
        if(drv_struct.Rx_data&VGS_LC){
            //do something
        }
        if(drv_struct.Rx_data&VGS_HC){
            //do something
        }
        if(drv_struct.Rx_data&VGS_LB){
            //do something
        }
        if(drv_struct.Rx_data&VGS_HB){
            //do something
        }
        if(drv_struct.Rx_data&VGS_LA){
            //do something
        }
        if(drv_struct.Rx_data&VGS_HA){
            //do something
        }
        if(drv_struct.Rx_data&CPUV){
            //do something
        }
        if(drv_struct.Rx_data&OTW){
            //do something
        }

        drv_struct.RdStatus=Read_free;
    }

}



/** @brief:probably useless for now */
void DRV8320_ShutDown(void){
	DRV8320S_SetENABLE(0);	
}
	
