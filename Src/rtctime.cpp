/*
 * RTCTime.cpp
 *
 *  Created on: May 27, 2022
 *      Author: vit
 */

#include "rtctime.h"

RTCTime::RTCTime() {
	// TODO Auto-generated constructor stub

}

RTCTime::~RTCTime() {
	// TODO Auto-generated destructor stub
}



void RTCTime::RTCDisableWP()
{
	RTC->WPR = 0xCA; /* (7) *///disable write protection
	RTC->WPR = 0x53; /* (7) */
}
void RTCTime::RTCEnableWP()
{
	RTC->WPR = 0xff;
	RTC->WPR = 0xff;
}
void RTCTime::RTCEnterInitMode()
{
	RTC->ICSR |= 0x00000080;//set INIT bit
	volatile uint32_t waitTimer = 0;
	while((RTC->ICSR & 0x00000040) != 0x00000040){waitTimer++; if(waitTimer>10000) return;};//waiting INITF flag
}
void RTCTime::RTCLeaveInitMode()
{
	RTC->ICSR &=~0x00000080;
}
void RTCTime::RTCInit(void)
{
	/* Enable the peripheral clock RTC */
  /* (1) Enable the LSI */
  /* (2) Wait while it is not ready */
  /* (3) Enable PWR clock */
  /* (4) Enable write in RTC domain control register */
  /* (5) LSI for RTC clock */
  /* (6) Disable PWR clock */
	  RCC->APBENR1 |= 0x10000000; /* (3) */ //PWREN
	  PWR->CR1 |= 0x00000100; /* (4) *///DBP

	  RCC->BDCR |= 0x00010000;//reset bit on
	  RCC->BDCR &= ~0x00010000;//reset bit off
	  //enable lsi
  RCC->CSR |= 0x00000001; /* (1) */
  volatile uint32_t waitTimer = 0;
  while((RCC->CSR & 0x00000002)!=0x00000002) /* (2) */
  {
	   waitTimer++; if(waitTimer>10000) return; /* add RTCTime out here for a robust application */
  }

  RCC->BDCR |= 0x00010000;//reset bit on
  RCC->BDCR &= ~0x00010000;//reset bit off

  RCC->BDCR |= 0x00000200;//set LSI as rtc clock source

  RCC->BDCR |= 0x00008000;// enable rtc
  RCC->APBENR1 |= 0x00000400;// enable rtc apb clock

  //RTC->WPR = 0xCA; /* (7) *///disable write protection
 // RTC->WPR = 0x53; /* (7) */

  RTCDisableWP();
  RTCEnterInitMode();

  //RTC->ICSR |= 0x00000080;//set INIT bit
  //while((RTC->ICSR & 0x00000080) != 0x00000080){};//waiting INITF flag

  //set hour format 24 hours
  RTC->CR &= ~ 0x00000040;
  //RTC->CR |= 0x00000020;//BYPSHAD
  //set sync prescaler 0xff
  RTC->PRER |= 0x000000ff;
  //set async prescaler 0x7f
  RTC->PRER |= 0x007f0000;

  RTC->CR &= ~0x00000300;//disable alarma & alarmb to enable write
waitTimer - 0;
  while((RTC->ICSR & 0x00000001) != 0x00000001) //wait while ALRAWF is set
  {
	  waitTimer++; if(waitTimer>10000) return;/* add time out here for a robust application */
  }
waitTimer = 0;
  while((RTC->ICSR & 0x00000002) != 0x00000002) //wait while ALRBWF is set
    {
	  waitTimer++; if(waitTimer>10000) return;/* add time out here for a robust application */
    }
  //8000000
  RTC->ALRMAR = 0x80808000;//Alarm A every minutes
  RTC->ALRMBR = 0x80808080;//Alarm B every second

  RTC->CR |= 0x00003300;//Enable alarmb alarma & interrupts


  RTCLeaveInitMode();
  //disable init mode
  //RTC->ICSR &=~0x00000080;


  //enable write protection
  RTCEnableWP();
  //RTC->WPR = 0xff;
  //RTC->WPR = 0xff;


  NVIC->ISER |= (1<<2);
}
//(dta/16)*10+dta%16;//bp 16 d 10
uint32_t RTCTime::timeToHw(const RTCTime::TimeNDate& tmdt)
{
	uint32_t RTCTimeReg = (tmdt.sec/10)*16+tmdt.sec%10;
	RTCTimeReg |= ((tmdt.min/10)*16+tmdt.min%10)<<8;
	RTCTimeReg |= (tmdt.hour%10)<<16;
	RTCTimeReg |= (tmdt.hour/10)<<20;
	return RTCTimeReg;
}

void RTCTime::timeFromHw(RTCTime::TimeNDate &tmdt, uint32_t reg)
{
	tmdt.sec = ((reg&0xff)/16)*10+(reg&0xff)%16;
	tmdt.min = (((reg&0xff00)>>8)/16)*10+((reg&0xff00)>>8)%16;
	tmdt.hour = (((reg&0xff0000)>>16)&0x0f)%10;
	if(reg&0x00100000)tmdt.hour+=10;
	if(reg&0x00200000)tmdt.hour+=20;
}
void RTCTime::setTime(const RTCTime::TimeNDate& tmdt)
{
	 uint32_t RTCTimeReg { 0 };
	 RTCTimeReg = timeToHw(tmdt);

	RTCDisableWP();
	RTCEnterInitMode();

	RTC->TR = RTCTimeReg;
	RTCLeaveInitMode();


	//if((RTC->CR & 0x00000020) == 0x00000020)
	//{
	//	while((RTC->ICSR & 0x00000020) != 0x00000020){}
	//}

	RTCEnableWP();

}

void RTCTime::getTime(RTCTime::TimeNDate &tmdt)
{
	uint32_t reg = RTC->TR;
	timeFromHw(tmdt,reg);
}

