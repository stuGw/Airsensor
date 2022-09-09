/*
 * time.h
 *
 *  Created on: May 27, 2022
 *      Author: vit
 */

#ifndef RTCTIME_H_
#define RTCTIME_H_
#include <stdint.h>
#include "STM32G030.h"

class RTCTime {
public:
	struct TimeNDate
		{
			uint16_t millSecs;
			uint8_t sec;
			uint8_t min;
			uint8_t hour;
			uint8_t wDay;
			uint8_t day;
			uint8_t month;
			uint16_t  year;
		} ;
private:
TimeNDate tmdt;
	void RTCDisableWP();
	void RTCEnableWP();
	void RTCEnterInitMode();
	void RTCLeaveInitMode();


	//(dta/16)*10+dta%16;//bp 16 d 10
	uint32_t timeToHw(const RTCTime::TimeNDate& tmdt);
	void timeFromHw(RTCTime::TimeNDate &tmdt, uint32_t reg);



public:
	RTCTime();
	virtual ~RTCTime();


	void RTCInit(void);
	void setTime(const RTCTime::TimeNDate& tmdt);
	void getTime(RTCTime::TimeNDate &tmdt);
};

#endif /* RTCTIME_H_ */
