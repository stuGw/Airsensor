// spi.h for stm32f030
// Author: Frank Duignan.  
// Updates posted on http://ioprog.com 

#include <stdint.h>
#include "STM32G030.h"
#include "spi.h"
void initSPI(void)
{
	int drain_count,drain;
	
	RCC->APBENR2 |= (1 << 12);		// turn on SPI1 	
	
	
	// GPIOA bits 5 and 7 are used for SPI1 (Alternative functions 0)
	RCC->IOPENR |= (1 << 0); // enable ports A
    GPIOA->MODER &= ~( (1 << 14)+(1 << 10) + (1 << 8)); // select Alternative function
    GPIOA->MODER |= ((1 << 15)+(1 << 11) + (1<<9));  // for bits 5,7 (not using MISO)
    GPIOA->AFRL &= 0x0f00ffff;		     // select Alt. Function 0
	
	// set port bits up as high speed outputs
	GPIOA->OSPEEDR |= (1 << 15) + (1 << 14) + (1 << 11) + (1 << 10) + (1<<9) + (1<<8);
	// Now configure the SPI interface
	drain = SPI1->SR;				// dummy read of SR to clear MODF
	// enable SSM, set SSI, enable SPI, PCLK/2, MSB First Master, Clock = 1 when idle
	//SPI1->CR1 = (1 << 8) + (1<<1) + (1<<9)+(1 << 6)+(1 << 2)   + ( 1<< 14);	 // update : set bit 5 to slow down the clock for debugging + bit 15 for bidimode + bit14 for tx mode

	//SPI1->CR1 = 0x38;//speed slowest
	//SPI1->CR1 |= 0x8000;
	//SPI1->CR1 |= 0x4000;

	SPI1->CR1|=0x4;//master mode

	SPI1->CR2  = 0x0700;//8 bit
	SPI1->CR2 |= 0x0004;//SSOE en

	SPI1->CR1 |= 0x0040;//Enable SPI

	//SPI1->CR2 = (1<<2) + (1 << 10)+(1 << 9)+(1 << 8); 	// configure for 8 bit operation
	//SPI1->CR1 |= 0xc03c ;//  (1 << 2);//  + (1<<8) + (1<<9);// + (1<<3) + (1<<4) + (1<<5);
	//SPI1->CR1 |= (1<<6);
   // for (drain_count = 0; drain_count < 32; drain_count++)
	//	drain = transferSPI8(0x00);
}

uint8_t transferSPI8(uint8_t data)
{
  //  unsigned Timeout = 100;
    *((uint8_t*)&SPI1->DR) = data;    
    while (((SPI1->SR & (1 << 7))!=0));//&&(Timeout--));
	return *((uint8_t*)&SPI1->DR);//*preg;	        
}

uint16_t transferSPI16(uint16_t data)
{
    unsigned Timeout = 100;    
    SPI1->DR = data;    
    while (((SPI1->SR & (1 << 7))!=0)&&(Timeout--));        
    return SPI1->DR;	
}
