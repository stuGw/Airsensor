/*
 * logger.h
 *
 *  Created on: May 27, 2022
 *      Author: vit
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include "serial.h"
#include "rtctime.h"

#define LOG Logger::getInstance()

class Logger {

	private:
	static Logger* p_instance;
	Logger(){}
	Logger( const Logger& );
	Logger& operator=( Logger& );
public:
	static Logger* getInstance() { if(!p_instance) p_instance = new Logger(); return p_instance; }

	 RTCTime *t{ nullptr };
	Serial* sout { nullptr };
	bool timeOn { true };

	void fillPower(float power, char *buff);
	void fillThermC(int16_t t, char *buff);
	void fillThermO(int8_t t, char *buff);
	void fillVGen(float vgen, char*buff);
public:

//	Logger();
//	virtual ~Logger();
	void DEBG(const char *String);
	void DEBG(const char *String, int number);
	void DEBG(const char *String, int number, int number2);
	void DEBGH(const char *String, int number);
	void DATA(const char *String);
	void stateMsg(const char *String);
	void stateMsg(float power, int16_t t1, int16_t t2, int8_t t3, float vgen, bool errt1, bool errt2, bool errt3);
	void setTimeShow(bool on){ timeOn = on; };
	void setSerial(Serial* serial){ sout = serial; }//HwOutFunction(void(*pf)(char c)){ printc = pf; }
	void setTimeSource(RTCTime* time){ t = time; }
	void prints(const char *String);
	void prints(const char *String, int number);
	void printsh(const char *String, int number);
	void printsfi(const char *String, int number, int number2);
	void printsf(const char *String, float number);
	void printTime();

};

#endif /* LOGGER_H_ */
