#include "Spi.h"

void triggerOutputData(){
     GPIO_ResetBits(GPIOA, NSS_PIN); // Clock Low
     GPIO_SetBits(GPIOA, NSS_PIN);   // Clock High
}

void SpiConfig(SPI_TypeDef* SPIx,
              uint16_t Mode,
              uint16_t DataSize,
              uint16_t BaudRatePrescaler,
              uint16_t FirstBit,
              uint16_t spi_clock
              ){

  if(SPIx == SPI1){RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);}
  else if(SPIx == SPI2){RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);}
  else if(SPIx == SPI3){RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);}

  SPI_InitTypeDef spi;
  spi.SPI_Mode = Mode;
  spi.SPI_DataSize = DataSize;
  spi.SPI_BaudRatePrescaler = BaudRatePrescaler;
  spi.SPI_FirstBit = FirstBit;
  switch(spi_clock){
    case 0:
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    break;
    case 1:
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_2Edge;
    break;
    case 2:
    spi.SPI_CPOL = SPI_CPOL_High;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    break;
    case 3:
    spi.SPI_CPOL = SPI_CPOL_High;
    spi.SPI_CPHA = SPI_CPHA_2Edge;
    break;
    default:break;
  }

  spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi.SPI_NSS = SPI_NSS_Hard;
  spi.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx,&spi);
  SPI_SSOutputCmd(SPIx,ENABLE);
  SPI_Cmd(SPIx,ENABLE);
}
