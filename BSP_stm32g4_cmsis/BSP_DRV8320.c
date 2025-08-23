#include "DRV8320S.h"
#include "BSP_DRV8320.h"




void BSP_Ctor(void){
  RCC->APB1ENR1|=RCC_APB2ENR_SPI1EN;
  SPI1->CR2|=SPI_CR2_DS;
  SPI1->CR1|=SPI_CR1_MSTR|SPI_CR1_SPE;
}


void BSP_Transmit(uint16_t addr,uint16_t reg){
  __BSP_SET_CS;
  uint16_t data = (addr)|(reg&DRV8320_DATA_MASK);
  SPI1->DR=data;
  //while(!(SPI1->SR&SPI_SR_TXE));/** useless command ,since spi has 4 byte buf */
  while(SPI1->SR&SPI_SR_BSY);
  if(SPI1->SR&SPI_SR_RXNE){
    data=SPI1->DR;
  }
  __BSP_RESET_CS;
}

uint16_t BSP_Receive(uint16_t addr){
  __BSP_SET_CS;
  uint16_t data= (addr)|0x8000;
  SPI1->DR=data;
  //while(!(SPI1->SR&SPI_SR_RXNE));/** useless , since bsy flag will turn off with rxne turn on */
  while(SPI1->SR&SPI_SR_BSY);
  __BSP_RESET_CS;
  return SPI1->DR;
}

DRV8320Functions FuncList={.Receive=&BSP_Receive
                            ,.Transmit=&BSP_Transmit
                            ,.Ctor=&BSP_Ctor};
