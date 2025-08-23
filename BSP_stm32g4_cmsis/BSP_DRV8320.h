#ifndef __BSP_DRV8320_H
#define __BSP_DRV8320_H
#include "stm32g474xx.h"


/** @brief: precalculated values for current config of gate drivers*/
#define IDRIVEN	0xA //>0.659A
#define IDRIVEP	0xD //>0.527A

#define __BSP_SET_CS        (GPIOB->BSRR=GPIO_BSRR_BR10)
#define __BSP_RESET_CS      (GPIOB->BSRR=GPIO_BSRR_BS10)
#define __BSP_SET_ENABLE    (GPIOA->BSRR=GPIO_BSRR_BS1)
#define __BSP_RESET_ENABLE  (GPIOA->BSRR=GPIO_BSRR_BR1)



typedef struct{
  void (*Ctor)(void);
  void (*Transmit)(uint16_t,uint16_t);
  uint16_t (*Receive)(uint16_t);
}DRV8320Functions;

extern DRV8320Functions FuncList;

#endif 
