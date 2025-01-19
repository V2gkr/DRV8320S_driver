


#include "DRV8320S.h"
#ifdef STM32G474xx
#include "BSP_DRV8320_STM32/BSP_DRV8320.h"
#endif

/** @brief: a struct to save data of driver*/
DRV_Struct drv_struct={.funcList=&FuncList};


/** @brief: initialization of regs */
void DRV8320S_Init(void){
	//while(!(FAULT_GPIO_Port->IDR&FAULT_Pin));
	drv_struct.RegStruct.DRV_CTRL_Reg=0;
	drv_struct.funcList->Ctor();
	drv_struct.funcList->Transmit(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
	//DRV8320S_Write_reg();
	
	
	DRV8320S_LockConfig(0);
	drv_struct.RegStruct.GDHS_Reg|=(IDRIVEP<<GATE_P_OFFSET)|IDRIVEN;
	drv_struct.funcList->Transmit(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
	
	drv_struct.RegStruct.GDLS_Reg|=(3<<GATE_LOCK_OFFSET)|(IDRIVEP<<GATE_P_OFFSET)|IDRIVEN;
	drv_struct.funcList->Transmit(DRV8320S_GATE_DRV_LS_1,drv_struct.RegStruct.GDLS_Reg);
	
	DRV8320S_LockConfig(1);
	
	//ocp mode 00b (vds latching)
	drv_struct.RegStruct.OCP_Control_Reg=DT_1|DT_0;
	drv_struct.funcList->Transmit(DRV8320S_OCP_CTRL_1,drv_struct.RegStruct.OCP_Control_Reg);
}



/** @brief:get status from both status registers*/
void DRV8320S_GetStatus(void){
	drv_struct.RegStruct.FAULT_Reg=drv_struct.funcList->Receive(DRV8320S_FAULT_STATUS_1);
	drv_struct.RegStruct.VGS_Reg=drv_struct.funcList->Receive(DRV8320S_VGS_STATUS_1);
}

/** @brief: clearing fault of registers */
void DRV8320S_clearFault(void){
	drv_struct.funcList->Transmit(DRV8320S_DRV_CTRL_1,drv_struct.RegStruct.DRV_CTRL_Reg&(~CLR_FLT));
}

/** @brief:locking a configuration of a driver registers */
void DRV8320S_LockConfig(uint8_t state){
	if(state){
		drv_struct.RegStruct.GDHS_Reg|=(6<<GATE_LOCK_OFFSET);
	}
	else{
		drv_struct.RegStruct.GDHS_Reg|=(3<<GATE_LOCK_OFFSET);
	}
	drv_struct.funcList->Transmit(DRV8320S_GATE_DRV_HS_1,drv_struct.RegStruct.GDHS_Reg);
}

/** @brief:that function is only to analyze content of received fault register */
void DRV8320S_ReceiveCheck(void){
  if(drv_struct.RdStatus==Fault_read_active){//a fault register may activate its read with fault pin too 
    if(drv_struct.Rx_data&FAULT){
      if(drv_struct.Rx_data&OTSD){
        drv_struct.faultFuncList->OTSD_FaultProcess();
      }
      if(drv_struct.Rx_data&UVLO){
        drv_struct.faultFuncList->UVLO_FaultProcess();
      }
      if(drv_struct.Rx_data&GDF){
        drv_struct.faultFuncList->GDF_FaultProcess();
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
  __BSP_RESET_ENABLE;
}
	
