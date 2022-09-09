#ifndef OL1331_H
#define OL1331_H

#include <stdint.h>
#include <stdlib.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COLOR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */









/*___________________________________________________________________________________________________*/


class OLED1331Display {


	volatile uint32_t* ioport { NULL };
	uint32_t DCMask { 0 };
	uint32_t ResMask { 0 };

	uint8_t cols { 0 };
	uint8_t rows { 0 };

	static constexpr uint16_t INIT_DELAY { 1000 };
	static constexpr uint16_t CLEAR_DELAY { 10000 };

	static constexpr uint8_t CMD_SET_COL_ADDR { 0x15 };
	static constexpr uint8_t CMD_SET_ROW_ADDR { 0x75 };

	void sendCommand(uint8_t comm);
	void sendData(uint8_t data);



	uint8_t (*transferSPI)(uint8_t);
public:
	OLED1331Display();
	enum Colors { BLUE = 0xf800, RED = 0x07e0, GREEN = 0x01f, BLACK = 0x0000, WHITE = 0xffff, YELLOW = 0xffe0, CYAN = 0x07ff, MAGENTA = 0xf81f };
	void setInterface(volatile uint32_t* port, uint32_t dcMask, uint32_t rsMask, uint8_t (*spif)(uint8_t));
	void init(uint8_t col, uint8_t row);
	void clear();
	void reset();
	uint16_t packColor(uint8_t r, uint8_t g, uint8_t b);
	void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
	void putPixel(uint32_t x, uint32_t y, uint16_t color);
	void putPixelNext(uint16_t color);
	void drawGraph(uint8_t* data, uint8_t x, uint8_t y);
	void drawCharFromGraphicArray(uint8_t c,  uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
	void drawStringUtf(const char* str, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
	void drawStringUtf(const char* str, int num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);


};


/*
//void setResetPin(uint32_t port, uint32_t pin);
//void setDCPin(uint32_t port, uint32_t pin);
void ol31SendCommand(char c);
void ol31SendData(char c);
void ol31Reset();
void ol31Init();
void ol31Clear();
void ol31DrawLine(char xs,char ys,char xe,char ye, unsigned short int color);
void ol31DrawRect(char xs,char ys,char xe,char ye, unsigned short int outColor, unsigned short int fillColor);
void ol31PutPixel(char x, char y, unsigned short int color);

void ol31Putc(char c , char x, char y, unsigned short int color);
void ol31Puts(const char* str,char x, char y, unsigned short int color);
void putPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

void putPixel(uint32_t x, uint32_t y, uint16_t color);
void putPixelNext(uint16_t color);
void drawCharUtf(wchar_t c, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
void drawStringUtf(const char* str, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
void drawStringUtf(const char* str, float num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
void drawStringUtf(const char* str, int num, uint8_t x, uint8_t y, uint16_t color, uint16_t bgrnd);
void drawGraph(uint8_t* data, uint8_t x, uint8_t y);*/
#endif //OL1331_H
