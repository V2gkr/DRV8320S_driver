
#include "BSP_DRV8320.h"
#include "stm32g474xx.h"

void BSP_SetCS(uint8_t state){
  if(state)
    GPIOB->BSRR=DRV_CS_Pin<<16;
  else
    GPIOB->BSRR=DRV_CS_Pin;
}

void BSP_SetEnable(uint8_t state){
  if(state)
    DRV_EN_GPIO_Port->BSRR=DRV_EN_Pin;
  else
    DRV_EN_GPIO_Port->BSRR=DRV_EN_Pin<<16;
}

void BSP_Transmit(uint16_t data){
  uint16_t data = (addr)|(reg&DRV8320_DATA_MASK);
  SPI1->DR=data;
  while(!(SPI1->SR&SPI_SR_TXE));
  while(SPI1->SR&SPI_SR_BSY);
  if(SPI1->SR&SPI_SR_RXNE){
    uint16_t dummy=SPI1->DR;
  }
}

uint16_t BSP_Receive(uint16_t addr){
  uint16_t data= (addr)|0x8000;
  SPI1->DR=data;
  while(!(SPI1->SR&SPI_SR_RXNE));
  while(SPI1->SR&SPI_SR_BSY);
  return SPI1->DR;
}

DRV8320Functions FuncList={.Receive=&BSP_Receive
                            ,.SetCS=&BSP_SetCS
                            ,.SetEnable=&BSP_SetEnable
                            ,.Transmit=&BSP_Transmit}