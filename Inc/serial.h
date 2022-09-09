#ifndef __serial_h
#define __serial_h
#define SBUFSIZE 2048
#include "rtctime.h"

typedef struct 
{
	volatile unsigned head,tail,count;
	char buffer[SBUFSIZE];
} SerialBuffer;

class Serial {
	const RTCTime* t{ nullptr };//if we need system realtime in output

public:
    Serial() {};
    void begin();
    void print(int Value);
    void print(char c);
    void print(const char *Str);
    void DEBG(const char *String);
    void DEBG(const char *String, int number);
    void stateMsg();
    void eputc(char ch);
    char egetc();
    int available();
    bool checkKey();
    bool checkDevices();
private:
    SerialBuffer TXBuffer,RXBuffer;
    friend void USART2_Handler(void);
    void setTimeSource(const RTCTime& time){ t = &time; }
    void putBuf(SerialBuffer &sbuf, char c);
    char getBuf(SerialBuffer &sbuf);
};
#endif
