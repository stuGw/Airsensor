/*
 * logger.cpp
 *
 *  Created on: May 27, 2022
 *      Author: vit
 */

#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rtctime.h"

Logger* Logger::p_instance = 0;
/*Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}
*/

void Logger::prints(const char *String)
{
    while(*String) // keep going until a null is encountered
    {
    	sout->eputc(*String); // write out the next character
        String++;       // move the pointer along
    }
}

void Logger::printsh(const char *String, int number)
{
    while(*String) // keep going until a null is encountered
    {
    	sout->eputc(*String); // write out the next character
        String++;       // move the pointer along
    }

    char buffer[12]; // ints can be +/- 2 billion (approx) so buffer needs to be this big
    buffer[11]=0;    // ensure the buffer is terminated with a null
    itoa(number,buffer,16);
    prints(buffer);
    //print(Buffer); // call string version of print
}
void Logger::prints(const char *String, int number)
{
    while(*String) // keep going until a null is encountered
    {
    	sout->eputc(*String); // write out the next character
        String++;       // move the pointer along
    }

    char buffer[12]; // ints can be +/- 2 billion (approx) so buffer needs to be this big
    buffer[11]=0;    // ensure the buffer is terminated with a null
    itoa(number,buffer,10);
    prints(buffer);
    //print(Buffer); // call string version of print
}
void Logger::printsfi(const char *String, int number, int number2)
{
    while(*String) // keep going until a null is encountered
    {
    	sout->eputc(*String); // write out the next character
        String++;       // move the pointer along
    }

    char buffer[12]; // ints can be +/- 2 billion (approx) so buffer needs to be this big
    char buffer2[12];
    buffer[11]=0;    // ensure the buffer is terminated with a null
    itoa(number,buffer,10);
    prints(buffer);

    buffer2[11]=0;    // ensure the buffer is terminated with a null
    itoa(number2,buffer2,10);
    sout->eputc('.');
    prints(buffer2);
    //print(Buffer); // call string version of print
}
void Logger::printTime()
{
	char tbuff[10] = {'[','9','9',':','9','9',':','9','9',']'};

	RTCTime::TimeNDate tmdt;
	if(t!=nullptr)
	{
		t->getTime(tmdt);

		tbuff[1] = tmdt.hour/10 + 0x30;
		tbuff[2] = tmdt.hour%10 + 0x30;
		tbuff[4] = tmdt.min/10 + 0x30;
		tbuff[5] = tmdt.min%10 + 0x30;
		tbuff[7] = tmdt.sec/10 + 0x30;
		tbuff[8] = tmdt.sec%10 + 0x30;
	}
	uint8_t i = 0;
	while(i<10)
	{
		sout->eputc(tbuff[i]); i++;}
}

void Logger::DEBG(const char *String)
{
	if(timeOn) printTime();
	prints("[DBG]");
	prints(String);
	sout->eputc('\n');
}
void Logger::DATA(const char *String)
{
	if(timeOn) printTime();
	prints("[DATA]");
	prints(String);
	sout->eputc('\n');
}
void Logger::DEBG(const char *String, int number)
{
	if(timeOn) printTime();
	prints("[DBG]");
	prints(String,number);
	sout->eputc('\n');
}
void Logger::DEBGH(const char *String, int number)
{
	if(timeOn) printTime();
	prints("[DBG]");
	printsh(String,number);
	sout->eputc('\n');
}
void Logger::DEBG(const char *String, int number, int number2)
{
	if(timeOn) printTime();
	prints("[DBG]");
	printsfi(String,number,number2);
	sout->eputc('\n');
}

static double PRECISION = 0.001;
static int MAX_NUMBER_STRING_SIZE = 32;

/**
 * Double to ASCII
 */
char * dtoa(char *s, double n) {
    // handle special cases
    if (isnan(n)) {
        strcpy(s, "nan");
    } else if (isinf(n)) {
        strcpy(s, "inf");
    } else if (n == 0.0) {
        strcpy(s, "0");
    } else {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
        // calculate magnitude
        m = log10(n);
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        // set up for scientific notation
        if (useExp) {
            if (m < 0)
               m -= 1.0;
            n = n / pow(10.0, m);
            m1 = m;
            m = 0;
        }
        if (m < 1.0) {
            m = 0;
        }
        // convert the number
        while (n > PRECISION || m >= 0) {
            double weight = pow(10.0, m);
            if (weight > 0 && !isinf(weight)) {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (m == 0 && n > 0)
                *(c++) = '.';
            m--;
        }
        if (useExp) {
            // convert the exponent
            int i, j;
            *(c++) = 'e';
            if (m1 > 0) {
                *(c++) = '+';
            } else {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0) {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--) {
                // swap without temporary
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
    }
    return s;
}

void Logger::printsf(const char *String, float number)
{
	char fstring[32];
	if(timeOn) printTime();
		prints("[DBG]");
		prints(String);

		dtoa(fstring, number);
		prints(fstring);

		sout->eputc('\n');
}

void Logger::fillPower(float power, char *buff)
{
	uint16_t powerD = power;
	float powerF = (power - ((float)powerD))*10.0;

	buff[0] = (powerD/100) + 0x30;
	powerD = powerD%100;
	buff[1] = (powerD/10) + 0x30;
	buff[2] = (powerD%10)+0x30;
	buff[4] =  ((uint8_t)powerF) + 0x30;

}

void Logger::fillThermC(int16_t t, char *buff)
{
	if(t<0){ buff[0] = '-'; t = t*-1; }else buff[0] = '+';
	buff[1] = (t/100) + 0x30;
	t = t%100;
	buff[2] = (t/10) + 0x30;
	buff[3] = (t%10)+0x30;

}

void Logger::fillThermO(int8_t t, char *buff)
{
	if(t<0)buff[0] = '-'; else buff[0] = '+';
	buff[1] = (t/100) + 0x30;
	t = t%100;
	buff[2] = (t/10) + 0x30;
	buff[3] = (t%10)+0x30;

}

void Logger::fillVGen(float vgen, char *buff)
{
	uint16_t vgenD = vgen;
	float vgenF = (vgen - ((float)vgenD))*10.0;

	buff[0] = (vgenD/10) + 0x30;
	buff[1] = (vgenD%10)+0x30;
	buff[3] =  ((uint8_t)vgenF) + 0x30;

}

void Logger::stateMsg(float power, int16_t t1, int16_t t2, int8_t t3, float vgen, bool errt1, bool errt2, bool errt3)
{
	char buffer[28] = {'{','P','O','W','.','R',',', 'T','H','C','1',',', 'T','H','C','2',',', 'T','O','S','E',',', 'V','G','.','N','}',0x00};

	fillPower(power,&buffer[1]);
	if(!errt1) fillThermC(t1,&buffer[7]); else { buffer[7] = 'e'; buffer[8] = 'r'; buffer[9] = 'r'; buffer[10] = '!';}
	if(!errt2) fillThermC(t2,&buffer[12]); else { buffer[12] = 'e'; buffer[13] = 'r'; buffer[14] = 'r'; buffer[15] = '!';}
	if(!errt3) fillThermO(t3,&buffer[17]); else { buffer[17] = 'e'; buffer[18] = 'r'; buffer[19] = 'r'; buffer[20] = '!';}
	fillVGen(vgen,&buffer[22]);

	if(timeOn) printTime();
		prints("[STATE]");
		prints(buffer);
		sout->eputc('\n');


}

void Logger::stateMsg(const char *String)
{
	if(timeOn) printTime();
	prints("[STATE]");
	prints(String);
	sout->eputc('\n');
}
