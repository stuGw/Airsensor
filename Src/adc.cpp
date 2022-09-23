#include "STM32G030.h"
#include "adc.h"
#include <stdint.h>

// This code reads the uncalibrated device internal temperature  


  uint32_t AnalogConverter::channelIndex;
  uint16_t AnalogConverter::countChannels;
  ADCChannel AnalogConverter::channels[];
  volatile uint16_t AnalogConverter::adcData[MAX_CHANNELS];
 void AnalogConverter::init()
 {
	 //start conversion by timer
	 RCC->APBENR1 |= 0x00000002;
	 TIM3->CR2 |= 0x0020;//TRGO by UPDate
	 TIM3->PSC = 4096;
	 TIM3->ARR = 4096;

	 RCC->APBENR2 |= (1 << 20); // enable ADC clock
	 ADC->CFGR2 |= (1 << 31); //
	 ADC->CFGR2 &= ~(1 << 30); // ADC Clock = HSI16/4
	 ADC->CR = 0;
	 ADC->CHSELR = (1 << 6);
	 ADC->CR = (1 << 28); // Turn on the voltage regulator
	 ADC->CCR |= (1 << 22) + (1 << 23);  // enable voltage reference and temperature sensor
	 ADC->CCR |= 0x00200000;//prescaler 32
	 volatile int dly=0;
	 while (dly < 1000) dly++; // wait for regulator to stabilize 20 microseconds
	 ADC->CR |= (1 << 31); // start calibration
	 while ( ADC->CR & (1 << 31) ); // wait for calibration to complete
	 ADC->SMPR = 0x07; // short S+H time


 }

 void AnalogConverter::initChannels(uint32_t chns)
 {
	 RCC->AHBENR |= 0x00000001;//enable DMA clock
	 RCC->IOPENR |= (1<<1);//enable port B
	 int i = 0;
	 uint32_t mask = 1;
	 while(i < MAX_CHANNELS)
	 {
		 if(chns & mask)
		 {
			 channels[countChannels].channelID = mask;
			 countChannels++;// = AnalogConverter::countChannels + 1;
		 }
		 mask = mask<<1;
		 i++;
	 }

	 DMA->CMAR1 = reinterpret_cast<uint32_t>(adcData);
	 DMA->CPAR1 = reinterpret_cast<uint32_t>(&(ADC->DR));

	 DMA->CNDTR1 = countChannels;
	 DMA->CCR1 |= 0x000005a0;//configure MSIZE 16bit, PSIZE 16bit, CIRC mode, MEmory increment

	 DMAMUX->C0CR |= 0x00000005;//ADC Request input
	 DMA->CCR1 |= 0x00000002;//transfer complete ipt enable
	 NVIC->ISER |= (1<<9);//enable ipt dma channel1
	 DMA->CCR1 |= 0x00000001;//enable

	 ADC->CR |= 1; // enable the ADC
	 ADC->CHSELR = chns;

	 ADC->CFGR1 |= 0x00002000;
	 ADC->CFGR1 |=0x00000002;//DMA circular mode
	 ADC->CFGR1 |= 0x00000001;//Enable DMA
	 ADC->CR |= 0x4;

 }

 void AnalogConverter::DMA_Handler()
 {
	 volatile static uint16_t count { 0 };
	 if(DMA->ISR & 0x00000002)
	 {

		 channels[0].summValue += adcData[0];
		 channels[1].summValue += adcData[1];
		 channels[2].summValue += adcData[2];
		 channels[3].summValue += adcData[3];
		 channels[4].summValue += adcData[4];
		 channels[5].summValue += adcData[5];
		 channels[6].summValue += adcData[6];
		 channels[7].summValue += adcData[7];
		 channels[8].summValue += adcData[8];
		 channels[9].summValue += adcData[9];
		 channels[10].summValue += adcData[10];
		 channels[11].summValue += adcData[11];
		 channels[12].summValue += adcData[12];
		 channels[13].summValue += adcData[13];
		 channels[14].summValue += adcData[14];

		 if(count == countSumm)
		 {
			 channels[0].filteredData = (uint16_t)(channels[0].summValue / (uint32_t)(count+1));
			 channels[0].summValue = channels[0].filteredData;

			 channels[1].filteredData = (uint16_t)(channels[1].summValue / (uint32_t)(count+1));
			 channels[1].summValue = channels[1].filteredData;

			 channels[2].filteredData = (uint16_t)(channels[2].summValue / (uint32_t)(count+1));
			 channels[2].summValue = channels[2].filteredData;

			 channels[3].filteredData = (uint16_t)(channels[3].summValue / (uint32_t)(count+1));
			 channels[3].summValue = channels[3].filteredData;

			 channels[4].filteredData = (uint16_t)(channels[4].summValue / (uint32_t)(count+1));
			 channels[4].summValue = channels[4].filteredData;

			 channels[5].filteredData = (uint16_t)(channels[5].summValue / (uint32_t)(count+1));
			 channels[5].summValue = channels[5].filteredData;

			 channels[6].filteredData = (uint16_t)(channels[6].summValue / (uint32_t)(count+1));
			 channels[6].summValue = channels[6].filteredData;

			 channels[7].filteredData = (uint16_t)(channels[7].summValue / (uint32_t)(count+1));
			 channels[7].summValue = channels[7].filteredData;

			 channels[8].filteredData = (uint16_t)(channels[8].summValue / (uint32_t)(count+1));
			 channels[8].summValue = channels[8].filteredData;

			 channels[9].filteredData = (uint16_t)(channels[9].summValue / (uint32_t)(count+1));
			 channels[9].summValue = channels[9].filteredData;

			 channels[10].filteredData = (uint16_t)(channels[10].summValue / (uint32_t)(count+1));
			 channels[10].summValue = channels[10].filteredData;

			 channels[11].filteredData = (uint16_t)(channels[11].summValue / (uint32_t)(count+1));
			 channels[11].summValue = channels[11].filteredData;

			 channels[12].filteredData = (uint16_t)(channels[12].summValue / (uint32_t)(count+1));
			 channels[12].summValue = channels[12].filteredData;

			 channels[13].filteredData = (uint16_t)(channels[13].summValue / (uint32_t)(count+1));
			 channels[13].summValue = channels[13].filteredData;

			 channels[14].filteredData = (uint16_t)(channels[14].summValue / (uint32_t)(count+1));
			 channels[14].summValue = channels[14].filteredData;
			 count = 0;
		 }
		 count++;
		 DMA->IFCR |= 0x00000002;//clear transfer complete flag
	 }

 }

 void AnalogConverter::start()
 {
	channelIndex = 0;
	 ADC->CR |= 0x4;
 }

