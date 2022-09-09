#include "oled1331.h"
#include "STM32G030.h"
//#include "lcd1202.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"

#include <string>
#include <sstream>
//#define INIT_DELAY 1000
//#define CLEAR_DELAY 10000


OLED1331Display::OLED1331Display()
{

}

void OLED1331Display::setInterface(volatile uint32_t* port, uint32_t dcMask, uint32_t rsMask, uint8_t (*spif)(uint8_t))
{
	ioport = port;
	DCMask = dcMask;
	ResMask = rsMask;
	transferSPI = spif;
}
void OLED1331Display::reset()
{
		int i = 0;
	*ioport &= ~ResMask;

	i = 0;
	while(i<160000)i++;

	*ioport |= ResMask;
	i = 0;
	while(i<160000)i++;

	*ioport &= ~DCMask;
}
void OLED1331Display::init(uint8_t col, uint8_t row)
{
	cols = col;
	rows = row;

	volatile int i = 0;
		//����������
	    sendCommand(0xAE);
	    i = 0; while(i<INIT_DELAY)i++;

		//��������� ���������
		//�������� �
	    sendCommand(0x81);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x91);i = 0; while(i<INIT_DELAY)i++;

		//�������� �
	    sendCommand(0x82);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x50);i = 0; while(i<INIT_DELAY)i++;

		//�������� �
	    sendCommand(0x83);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x7D);i = 0; while(i<INIT_DELAY)i++;

		//Master current control
	    sendCommand(0x87);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x06);i = 0; while(i<INIT_DELAY)i++;

		//Precharge A
	    sendCommand(0x8A);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x64);i = 0; while(i<INIT_DELAY)i++;

		//Precharge B
	    sendCommand(0x8B);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x78);i = 0; while(i<INIT_DELAY)i++;

		//Precharge C
	    sendCommand(0x8C);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x64);i = 0; while(i<INIT_DELAY)i++;

		//Remap
	    sendCommand(0xA0);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x72);i = 0; while(i<INIT_DELAY)i++;

		//��������� ��������� �����
	    sendCommand(0xA1);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;

		//�������� �� �����
	    sendCommand(0xA2);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;

		//��������� ������ �������
	    sendCommand(0xA4);i = 0; while(i<INIT_DELAY)i++;

		//��������� ������������ �������������
	    sendCommand(0xA8);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x3F);i = 0; while(i<INIT_DELAY)i++;

		//Master configure
	    sendCommand(0xAD);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x8E);i = 0; while(i<INIT_DELAY)i++;

		//����� ����������������
	    sendCommand(0xB0);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;

	    //Adjustment
	    sendCommand(0xB1);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x31);i = 0; while(i<INIT_DELAY)i++;

		//�������� ������� �������
	    sendCommand(0xB3);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0xF0);i = 0; while(i<INIT_DELAY)i++;

		//Precharge voltage
	    sendCommand(0xBB);i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x3A);i = 0; while(i<INIT_DELAY)i++;

		//V voltage
	    sendCommand(0xBE);  i = 0; while(i<INIT_DELAY)i++;
	    sendCommand(0x3E);i = 0; while(i<INIT_DELAY)i++;

		//��������� ���������
	    sendCommand(0x2E);i = 0; while(i<INIT_DELAY)i++;

		//�������� �������
	    sendCommand(0xAF);i = 0; while(i<INIT_DELAY)i++;

}

void OLED1331Display::sendCommand(uint8_t comm)
{
	*ioport &=~ DCMask;
	transferSPI(comm);
}

void OLED1331Display::sendData(uint8_t data)
{
	*ioport |= DCMask;
	transferSPI(data);
}

void OLED1331Display::clear()
{
	volatile int i = 0;
	//clear command
	sendCommand(0x25);i = 0; while(i<CLEAR_DELAY)i++;

	//xstart
	sendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//ystart
	sendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//xend
	sendCommand(95);i = 0; while(i<CLEAR_DELAY)i++;
	//yend
	sendCommand(63);i = 0; while(i<CLEAR_DELAY)i++;

}

uint16_t OLED1331Display::packColor(uint8_t r, uint8_t g, uint8_t b)
{

	//return (uint16_t)(b & 0x1f)<<8 | (((uint16_t)(g & 0x3f))<<5) | (((uint16_t)(r & 0x1f))<<11);
	return ((g >> 3) & 0x7) | (r << 3) | (((uint16_t)b)<<8) | (((uint16_t)(g & 3))<<13);
}

void OLED1331Display::putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
	if((x>=cols) || (y >= rows)) return;

	sendCommand(CMD_SET_COL_ADDR);
	sendCommand(x);//start
	sendCommand(cols - 1);//end

	sendCommand(CMD_SET_ROW_ADDR);

	sendCommand(y);//start
	sendCommand(rows - 1);//end

    uint16_t color = packColor(r,g,b);

    sendData(static_cast<uint8_t>(color&0xff));
    sendData(static_cast<uint8_t>((color>>8)&0xff));
}

void OLED1331Display::putPixel(uint32_t x, uint32_t y, uint16_t color)
{
	if((x>=cols) || (y >= rows)) return;

	sendCommand(CMD_SET_COL_ADDR);
	sendCommand(x);//start
	sendCommand(cols-1);//end

	sendCommand(CMD_SET_ROW_ADDR);

	sendCommand(y);//start
	sendCommand(rows-1);//end

    sendData(static_cast<uint8_t>(color&0xff));
    sendData(static_cast<uint8_t>((color>>8)&0xff));
}


void OLED1331Display::putPixelNext(uint16_t color)
{
	sendData(static_cast<uint8_t>(color&0xff));
	sendData(static_cast<uint8_t>((color>>8)&0xff));
}
void OLED1331Display::drawGraph(uint8_t* data, uint8_t x, uint8_t y)
{
	volatile int i = 0;
	//clear command
	sendCommand(0x25);i = 0; while(i<CLEAR_DELAY)i++;

	//xstart
	sendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//ystart
	sendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//xend
	sendCommand(95);i = 0; while(i<CLEAR_DELAY)i++;
	//yend
	sendCommand(55);i = 0; while(i<CLEAR_DELAY)i++;
	i = 0;
	while(i<x)
	{
		if(data[i] < y)putPixel(i,data[i],GREEN);
		i++;
	}
}

void OLED1331Display::drawCharFromGraphicArray(uint8_t c,  uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	for(int i = 0; i<5;i++)
		{
			uint8_t mask = 0x01;

			for(int k = 0;k<8;k++)
			{
				if(charsEn[c][i]&(mask<<k))
				{
					//lcd->displayPtr[x+(y+k)*DISPLAY_WIDTH+i] = color;
					putPixel(x+i, y+k, color);
				}
				else
				{
					//lcd->displayPtr[x+(y+k)*DISPLAY_WIDTH+i] = bgrnd;
					putPixel(x+i, y+k, bgrnd);
				}
			}
		}
}

void OLED1331Display::drawStringUtf(const char* str, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	int strSize = strlen(str);

	for(int i = 0,j = 0;i<strSize;i++,j++)
	{
		uint8_t indexInGraphicArray = 0;
		char saved = str[i];
		unsigned short int value = 0;
		if(str[i]>122)
		{
			value = str[i];
			value = value<<8;
			i++;
			value|=str[i];
			if(saved == CHARACTER_UTFRUS_SIGNOFFS1)
			indexInGraphicArray = value-CHARACTER_UTFRUS_OFFSET1+GRAPHICFONT_RUSOFFS1;
			else
		    indexInGraphicArray = value-CHARACTER_UTFRUS_OFFSET2+GRAPHICFONT_RUSOFFS2;
		}else
		{
			indexInGraphicArray = str[i]-GRAPHICFONT_ASCIIENGOFFS;
		}
	    drawCharFromGraphicArray(indexInGraphicArray,x+j*(GRAPHICFONT_CHAR_W+GRAPHICFONT_CHARSPACE_INPIX),y,color,bgrnd);
	}
}


void OLED1331Display::drawStringUtf(const char* str, int num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	char string[17];
	//std::string string = static_cast<std::ostringstream*>( &(std::ostringstream() << num) )->str();
	drawStringUtf(str, x, y,color,bgrnd);
	itoa(num,string,10);
	drawStringUtf("    ",x+strlen(str)*GRAPHICFONT_CHARSPACE_INPIX*2, y,bgrnd,bgrnd);
	drawStringUtf(string, x+strlen(str)*GRAPHICFONT_CHARSPACE_INPIX*2, y,color,bgrnd);
}

/*
void ol31SendCommand(char c)
{
	int i = 0;
	
	OL_DC0;
	
	#ifdef PIN_CS
	OL_CS0;
	#endif
	
	//spi1Sent(c);
	transferSPI8(c);
	
	#ifdef PIN_CS
	OL_CS1;
	#endif
	
	i = 0;
	//while(i<1600)i++;
	
}

void ol31SendData(char c)
{

	OL_DC1;
	
	#ifdef PIN_CS
	OL_CS0;
	#endif
	
	transferSPI8(c);
	
	#ifdef PIN_CS
	OL_CS1;
	#endif
	
}

void ol31Init()
{
	volatile int i = 0;
	//����������
    ol31SendCommand(0xAE);
    i = 0; while(i<INIT_DELAY)i++;
	
	//��������� ���������
	//�������� �
    ol31SendCommand(0x81);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x91);i = 0; while(i<INIT_DELAY)i++;
	
	//�������� �
    ol31SendCommand(0x82);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x50);i = 0; while(i<INIT_DELAY)i++;
	
	//�������� �
    ol31SendCommand(0x83);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x7D);i = 0; while(i<INIT_DELAY)i++;
    
	//Master current control
    ol31SendCommand(0x87);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x06);i = 0; while(i<INIT_DELAY)i++;
	
	//Precharge A
    ol31SendCommand(0x8A);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x64);i = 0; while(i<INIT_DELAY)i++;
	
	//Precharge B
    ol31SendCommand(0x8B);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x78);i = 0; while(i<INIT_DELAY)i++;
	
	//Precharge C
    ol31SendCommand(0x8C);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x64);i = 0; while(i<INIT_DELAY)i++;
	
	//Remap
    ol31SendCommand(0xA0);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x72);i = 0; while(i<INIT_DELAY)i++;
	
	//��������� ��������� �����
    ol31SendCommand(0xA1);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;
	
	//�������� �� �����
    ol31SendCommand(0xA2);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;
    
	//��������� ������ �������
    ol31SendCommand(0xA4);i = 0; while(i<INIT_DELAY)i++;
	
	//��������� ������������ �������������
    ol31SendCommand(0xA8);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x3F);i = 0; while(i<INIT_DELAY)i++;
	
	//Master configure
    ol31SendCommand(0xAD);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x8E);i = 0; while(i<INIT_DELAY)i++;
	
	//����� ����������������
    ol31SendCommand(0xB0);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x00);i = 0; while(i<INIT_DELAY)i++;
	
    //Adjustment
    ol31SendCommand(0xB1);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x31);i = 0; while(i<INIT_DELAY)i++;
	
	//�������� ������� �������
    ol31SendCommand(0xB3);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0xF0);i = 0; while(i<INIT_DELAY)i++;
	
	//Precharge voltage
    ol31SendCommand(0xBB);i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x3A);i = 0; while(i<INIT_DELAY)i++;
	
	//V voltage
    ol31SendCommand(0xBE);  i = 0; while(i<INIT_DELAY)i++;
    ol31SendCommand(0x3E);i = 0; while(i<INIT_DELAY)i++;
	
	//��������� ���������
    ol31SendCommand(0x2E);i = 0; while(i<INIT_DELAY)i++;
	
	//�������� �������
    ol31SendCommand(0xAF);i = 0; while(i<INIT_DELAY)i++;

    
}

void ol31Clear()
{
	volatile int i = 0;
	//clear command
	ol31SendCommand(0x25);i = 0; while(i<CLEAR_DELAY)i++;

	//xstart
	ol31SendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//ystart
	ol31SendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//xend
	ol31SendCommand(95);i = 0; while(i<CLEAR_DELAY)i++;
	//yend
	ol31SendCommand(63);i = 0; while(i<CLEAR_DELAY)i++;
	
	
}
*/
/*
void ol31DrawLine(char xs,char ys,char xe,char ye, unsigned short int color)
{
unsigned short int cv;
	
	cv = RED;
	//draw line
	ol31SendCommand(0x21);
	
	//xstart
	ol31SendCommand(xs);
	//ystart
	ol31SendCommand(ys);
	//xend
	ol31SendCommand(xe);
	//yend
	ol31SendCommand(ye);
	
	//color
	ol31SendCommand((char)((color>>4)&0x000000ff));
	//G
	ol31SendCommand((char)((color>>2)&0x000000ff));
	//B
	ol31SendCommand((char)((color)&0x000000ff));
	

	
}

void ol31DrawRect(char xs,char ys,char xe,char ye, unsigned short int outColor, unsigned short int fillColor)
{

	//draw rect
	ol31SendCommand(0x22);
	
	//xstart
	ol31SendCommand(xs);
	//ystart
	ol31SendCommand(ys);
	//xend
	ol31SendCommand(xe);
	//yend
	ol31SendCommand(ye);
	
	//color outline
	//R
	ol31SendCommand((char)(color&RED));
	//G
	ol31SendCommand((char)(color&GREEN));
	//B
	ol31SendCommand(0x00);
	
	//color fill
	//R
	ol31SendCommand((char)(color&RED));
	//G
	ol31SendCommand((char)(color&GREEN));
	//B
	ol31SendCommand((char)(color&BLUE));

}

uint16_t packColor(uint8_t r, uint8_t g, uint8_t b)
{

	//return (uint16_t)(b & 0x1f)<<8 | (((uint16_t)(g & 0x3f))<<5) | (((uint16_t)(r & 0x1f))<<11);
	return ((g >> 3) & 0x7) | (r << 3) | (((uint16_t)b)<<8) | (((uint16_t)(g & 3))<<13);
}

void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
	if((x>=WIDTH) || (y >= HEIGHT)) return;

	ol31SendCommand(CMD_SET_COL_ADDR);
	ol31SendCommand(x);//start
	ol31SendCommand(LAST_PIXEL_COL);//end

	ol31SendCommand(CMD_SET_ROW_ADDR);

	ol31SendCommand(y);//start
	ol31SendCommand(LAST_PIXEL_ROW);//end

    uint16_t color = packColor(r,g,b);

    ol31SendData(static_cast<uint8_t>(color&0xff));
    ol31SendData(static_cast<uint8_t>((color>>8)&0xff));
}

void putPixel(uint32_t x, uint32_t y, uint16_t color)
{
	if((x>=WIDTH) || (y >= HEIGHT)) return;

	ol31SendCommand(CMD_SET_COL_ADDR);
	ol31SendCommand(x);//start
	ol31SendCommand(LAST_PIXEL_COL);//end

	ol31SendCommand(CMD_SET_ROW_ADDR);

	ol31SendCommand(y);//start
	ol31SendCommand(LAST_PIXEL_ROW);//end

    ol31SendData(static_cast<uint8_t>(color&0xff));
    ol31SendData(static_cast<uint8_t>((color>>8)&0xff));
}

void putPixelNext(uint16_t color)
{
	ol31SendData(static_cast<uint8_t>(color&0xff));
	ol31SendData(static_cast<uint8_t>((color>>8)&0xff));
}
void drawGraph(uint8_t* data, uint8_t x, uint8_t y)
{
	volatile int i = 0;
	//clear command
	ol31SendCommand(0x25);i = 0; while(i<CLEAR_DELAY)i++;

	//xstart
	ol31SendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//ystart
	ol31SendCommand(0);i = 0; while(i<CLEAR_DELAY)i++;
	//xend
	ol31SendCommand(95);i = 0; while(i<CLEAR_DELAY)i++;
	//yend
	ol31SendCommand(55);i = 0; while(i<CLEAR_DELAY)i++;
	i = 0;
	while(i<x)
	{
		if(data[i] < y)putPixel(i,data[i],BLUE);
		i++;
	}
}
void drawCharUtf(wchar_t c, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{

	if(c>143)c = c-144;
	for(int i = 0; i<5;i++)
	{
		if(charsEn[c][i]&0x01)putPixel(x, y, color); else  putPixel(x, y, bgrnd);
		if(charsEn[c][i]&0x02)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x04)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x08)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x10)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x20)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x40)putPixelNext(color); else  putPixelNext(bgrnd);
		if(charsEn[c][i]&0x80)putPixelNext(color); else  putPixelNext(bgrnd);
	}
}
void drawCharFromGraphicArray(uint8_t c,  uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	for(int i = 0; i<5;i++)
		{
			uint8_t mask = 0x01;

			for(int k = 0;k<8;k++)
			{
				if(charsEn[c][i]&(mask<<k))
				{
					//lcd->displayPtr[x+(y+k)*DISPLAY_WIDTH+i] = color;
					putPixel(x+i, y+k, color);
				}
				else
				{
					//lcd->displayPtr[x+(y+k)*DISPLAY_WIDTH+i] = bgrnd;
					putPixel(x+i, y+k, bgrnd);
				}
			}
		}
}

void drawStringUtf(const char* str, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	int strSize = strlen(str);

	for(int i = 0,j = 0;i<strSize;i++,j++)
	{
		uint8_t indexInGraphicArray = 0;
		char saved = str[i];
		unsigned short int value = 0;
		if(str[i]>122)
		{
			value = str[i];
			value = value<<8;
			i++;
			value|=str[i];
			if(saved == CHARACTER_UTFRUS_SIGNOFFS1)
			indexInGraphicArray = value-CHARACTER_UTFRUS_OFFSET1+GRAPHICFONT_RUSOFFS1;
			else
		    indexInGraphicArray = value-CHARACTER_UTFRUS_OFFSET2+GRAPHICFONT_RUSOFFS2;
		}else
		{
			indexInGraphicArray = str[i]-GRAPHICFONT_ASCIIENGOFFS;
		}
	    drawCharFromGraphicArray(indexInGraphicArray,x+j*(GRAPHICFONT_CHAR_W+GRAPHICFONT_CHARSPACE_INPIX),y,color,bgrnd);
	}
}

void drawStringUtf(const char* str, float num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{

}

void drawStringUtf(const char* str, int num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd)
{
	char string[17];
	//std::string string = static_cast<std::ostringstream*>( &(std::ostringstream() << num) )->str();
	drawStringUtf(str, x, y,color,bgrnd);
	itoa(num,string,10);
	drawStringUtf("    ",x+strlen(str)*GRAPHICFONT_CHARSPACE_INPIX*2, y,bgrnd,bgrnd);
	drawStringUtf(string, x+strlen(str)*GRAPHICFONT_CHARSPACE_INPIX*2, y,color,bgrnd);
}

void ol31PutPixel(char x, char y, unsigned short int color)
{
	ol31DrawLine(x,y,x,y,color);
}


void ol31Putc(char c , char x, char y, unsigned short int color)
{

	
}



void ol31Puts(const char* str,char x, char y, unsigned short int color)
{
	int i = 0;
	
	while(str[i]!='\0')
	{
		ol31Putc(str[i],x,y,color);
		
		x+=6;
		
		if(x>95)
		{
			y+=8;
			
			x = 0;
			
			if(y>63)
				return;
		}
		
		i++;
	}
}




void ol31Reset()
{
		int i = 0;    
	OL_RES0;

	i = 0;
	while(i<160000)i++;
	//delay_us(300);//?

	OL_RES1;
	i = 0;
	while(i<160000)i++;
	//delay_us(300);//?

	OL_DC0;
}
*/
