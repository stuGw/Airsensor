#ifndef OL1306_H
#define OL1306_H

#include <stdint.h>
#include <stdlib.h>

#include <string>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COLOR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*___________________________________________________________________________________________________*/


class OLED1306Display {


	volatile uint32_t* ioport { NULL };
	uint32_t DCMask { 0 };
	uint32_t ResMask { 0 };

	uint8_t cols { 0 };
	uint8_t rows { 0 };

	static constexpr uint16_t INIT_DELAY { 1000 };
	static constexpr uint16_t CLEAR_DELAY { 10000 };

	static constexpr uint8_t CMD_SET_COL_ADDR { 0x15 };
	static constexpr uint8_t CMD_SET_ROW_ADDR { 0x75 };
	static constexpr uint8_t PIXEL_PER_ROW { 8 };

	void sendCommand(uint8_t comm);
	void sendData(uint8_t data);
	void sendDataArray(uint8_t* arr, uint16_t count);

	uint8_t buffer[128*8];
	uint8_t (*transferSPI)(uint8_t);
public:
	OLED1306Display();
	enum Colors { BLUE = 0xf800, RED = 0x07e0, GREEN = 0x01f, BLACK = 0x0000, WHITE = 0xffff, YELLOW = 0xffe0, CYAN = 0x07ff, MAGENTA = 0xf81f };
	void setInterface(volatile uint32_t* port, uint32_t dcMask, uint32_t rsMask, uint8_t (*spif)(uint8_t));
	void init(uint8_t col, uint8_t row);
	void clear();
	void reset();
	uint16_t packColor(uint8_t r, uint8_t g, uint8_t b);
	void putPixel(uint32_t x, uint32_t y);
	void putPixel(uint32_t x, uint32_t y, uint16_t color);
	void putPixelNext(uint16_t color);
	void drawGraph(uint8_t* data, uint8_t x, uint8_t y);
	void drawCharFromGraphicArray(uint8_t c,  uint8_t x, uint8_t y);
	void drawStringUtf(const char* str, uint8_t x, uint8_t y);
	void drawStringUtf(const char* str, int num, uint8_t x, uint8_t y);
	void drawString(std::string str, uint8_t x, uint8_t y);
	void fill(uint8_t color);
	void refresh();

};

#endif //OL1306_H
