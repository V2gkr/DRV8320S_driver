#include "DRV8320S.h"
#include "BSP_DRV8320.h"




void BSP_Ctor(void){
  RCC->APB1ENR1|=RCC_APB1ENR1_SPI3EN;
  SPI3->CR2|=SPI_CR2_DS;
  SPI3->CR1|=SPI_CR1_MSTR|SPI_CR1_SPE;
}


void BSP_Transmit(uint16_t addr,uint16_t reg){
  __BSP_SET_CS;
  uint16_t data = (addr)|(reg&DRV8320_DATA_MASK);
  SPI3->DR=data;
  //while(!(SPI3->SR&SPI_SR_TXE));/** useless command ,since spi has 4 byte buf */
  while(SPI3->SR&SPI_SR_BSY);
  if(SPI3->SR&SPI_SR_RXNE){
    data=SPI3->DR;
  }
  __BSP_RESET_CS;
}

uint16_t BSP_Receive(uint16_t addr){
  __BSP_SET_CS;
  uint16_t data= (addr)|0x8000;
  SPI3->DR=data;
  //while(!(SPI3->SR&SPI_SR_RXNE));/** useless , since bsy flag will turn off with rxne turn on */
  while(SPI3->SR&SPI_SR_BSY);
  __BSP_RESET_CS;
  return SPI3->DR;
}

DRV8320Functions FuncList={.Receive=&BSP_Receive
                            ,.Transmit=&BSP_Transmit
                            ,.Ctor=&BSP_Ctor};
