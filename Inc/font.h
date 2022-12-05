/*
 * font.h
 *
 *  Created on: Apr 8, 2022
 *      Author: vit
 */

#ifndef FONT_H_
#define FONT_H_


#define CHARACTER_UTFRUS_OFFSET1  53392
#define CHARACTER_UTFRUS_OFFSET2  53632
#define CHARACTER_UTFRUS_SIGNOFFS1  208

#define GRAPHICFONT_RUSOFFS1  94
#define GRAPHICFONT_RUSOFFS2  142

#define GRAPHICFONT_ASCIIENGOFFS  32
#define GRAPHICFONT_CHARSPACE_INPIX  2
#define GRAPHICFONT_CHAR_W  5

#define SSIGN_PRESSURE1 0x01
#define SSIGN_PRESSURE2 0x02
#define SSIGN_GRADUS 0x03
#define SSIGN_HUMIDITY 0x04
#define SSIGN_POWER 0x5

#define COUNT_SPECIAL 5
#define COUNT_SPECIAL_8x16 4
static const unsigned char chars8x16[26+COUNT_SPECIAL_8x16][16] = {
		 0x80, 0x40, 0x40, 0xac, 0x12, 0x12, 0x0c, 0x00, 0x01, 0x02, 0x02, 0x05, 0x68, 0x90, 0x90, 0x60, // C 35
		  0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, // C 35
		    0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, // O 47
			0x00, 0x62, 0x51, 0x11, 0x49, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 2 52

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //   0
     0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00, // ! 1
     0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " 2
     0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00, // # 3
     0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00, // $ 4
     0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00, // % 5
     0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10, // & 6
     0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' 7
     0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, // ( 8
     0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, // ) 9
     0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00, // * 10
     0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00, // + 11
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, // , 12
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // - 13
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, // . 14
     0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, // / 15
     0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00, // 0 16
     0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, // 1 17
     0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00, // 2 18
     0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, // 3 19
     0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00, // 4 20
     0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00, // 5 21
     0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, // 6 22
     0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, // 7 23
     0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00, // 8 24
     0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00 // 9 25
};

static const unsigned char charsEn[158+COUNT_SPECIAL][5] = {

		//special sumbols
					{0x14, 0x3e, 0x55, 0x3e, 0x14}, //я	0xC0
					{0x20, 0x7f, 0x20, 0x7f, 0x20}, //я	0xC1

					{0x38, 0x7e, 0x5f, 0x6e, 0x38}, //я	0xC2
					{0x06, 0x09, 0x09, 0x06, 0x00}, //я	0xC3
					{0x88, 0x4c, 0x3e, 0x19, 0x08}, //я	0xC3

					0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,




	{0x7e, 0x11, 0x11, 0x11, 0x7e},//A	0x80
	{0x7f, 0x49, 0x49, 0x49, 0x33},//Б	0x81
	{0x7f, 0x49, 0x49, 0x49, 0x36},//В	0x82
	{0x7f, 0x01, 0x01, 0x01, 0x03},//Г	0x83
			{0xe0, 0x51, 0x4f, 0x41, 0xff},//Д	0x84
			{0x7f, 0x49, 0x49, 0x49, 0x41},//E	0x85
			{0x77, 0x08, 0x7f, 0x08, 0x77},//Ж	0x86
			{0x41, 0x49, 0x49, 0x49, 0x36},//З	0x87
			{0x7f, 0x10, 0x08, 0x04, 0x7f},//И	0x88
			{0x7c, 0x21, 0x12, 0x09, 0x7c},//Й	0x89
			{0x7f, 0x08, 0x14, 0x22, 0x41},//K	0x8A
			{0x20, 0x41, 0x3f, 0x01, 0x7f},//Л	0x8B
			{0x7f, 0x02, 0x0c, 0x02, 0x7f},//M	0x8C
			{0x7f, 0x08, 0x08, 0x08, 0x7f},//H	0x8D
			{0x3e, 0x41, 0x41, 0x41, 0x3e},//O	0x8E
			{0x7f, 0x01, 0x01, 0x01, 0x7f},//П	0x8F
			{0x7f, 0x09, 0x09, 0x09, 0x06},//P	0x90
			{0x3e, 0x41, 0x41, 0x41, 0x22},//C	0x91
			{0x01, 0x01, 0x7f, 0x01, 0x01},//T	0x92
			{0x47, 0x28, 0x10, 0x08, 0x07},//У	0x93
			{0x1c, 0x22, 0x7f, 0x22, 0x1c},//Ф	0x94
			{0x63, 0x14, 0x08, 0x14, 0x63},//X	0x95
			{0x7f, 0x40, 0x40, 0x40, 0xff},//Ц	0x96
			{0x07, 0x08, 0x08, 0x08, 0x7f},//Ч	0x97
			{0x7f, 0x40, 0x7f, 0x40, 0x7f},//Ш	0x98
			{0x7f, 0x40, 0x7f, 0x40, 0xff},//Щ	0x99
			{0x01, 0x7f, 0x48, 0x48, 0x30},//Ъ	0x9A
			{0x7f, 0x48, 0x30, 0x00, 0x7f},//Ы	0x9B
			{0x00, 0x7f, 0x48, 0x48, 0x30},//Э	0x9C
			{0x22, 0x41, 0x49, 0x49, 0x3e},//Ь	0x9D
			{0x7f, 0x08, 0x3e, 0x41, 0x3e},//Ю	0x9E
			{0x46, 0x29, 0x19, 0x09, 0x7f},//Я	0x9F
		// маленькие буквы
		 	{0x20, 0x54, 0x54, 0x54, 0x78},//a	0xA0
			{0x3c, 0x4a, 0x4a, 0x49, 0x31},//б	0xA1
			{0x7c, 0x54, 0x54, 0x28, 0x00},//в	0xA2
			{0x7c, 0x04, 0x04, 0x04, 0x0c},//г	0xA3
			{0xe0, 0x54, 0x4c, 0x44, 0xfc},//д	0xA4
			{0x38, 0x54, 0x54, 0x54, 0x18},//e	0xA5
			{0x6c, 0x10, 0x7c, 0x10, 0x6c},//ж	0xA6
			{0x44, 0x44, 0x54, 0x54, 0x28},//з	0xA7
			{0x7c, 0x20, 0x10, 0x08, 0x7c},//и	0xA8
			{0x7c, 0x41, 0x22, 0x11, 0x7c},//й	0xA9
			{0x7c, 0x10, 0x28, 0x44, 0x00},//к	0xAA
			{0x20, 0x44, 0x3c, 0x04, 0x7c},//л	0xAB
			{0x7c, 0x08, 0x10, 0x08, 0x7c},//м	0xAC
			{0x7c, 0x10, 0x10, 0x10, 0x7c},//н	0xAD
			{0x38, 0x44, 0x44, 0x44, 0x38},//o	0xAE
			{0x7c, 0x04, 0x04, 0x04, 0x7c},//п	0xAF
			{0x7C, 0x14, 0x14, 0x14, 0x08},//p 	0xB0
			{0x38, 0x44, 0x44, 0x44, 0x20},//c	0xB1
			{0x04, 0x04, 0x7c, 0x04, 0x04},//т	0xB2
			{0x0C, 0x50, 0x50, 0x50, 0x3C},//у	0xB3
			{0x30, 0x48, 0xfc, 0x48, 0x30},//ф	0xB4
			{0x44, 0x28, 0x10, 0x28, 0x44},//x	0xB5
			{0x7c, 0x40, 0x40, 0x40, 0xfc},//ц	0xB6
			{0x0c, 0x10, 0x10, 0x10, 0x7c},//ч	0xB7
			{0x7c, 0x40, 0x7c, 0x40, 0x7c},//ш	0xB8
			{0x7c, 0x40, 0x7c, 0x40, 0xfc},//щ	0xB9
			{0x04, 0x7c, 0x50, 0x50, 0x20},//ъ	0xBA
			{0x7c, 0x50, 0x50, 0x20, 0x7c},//ы	0xBB
			{0x7c, 0x50, 0x50, 0x20, 0x00},//ь	0xBC
			{0x28, 0x44, 0x54, 0x54, 0x38},//э	0xBD
			{0x7c, 0x10, 0x38, 0x44, 0x38},//ю	0xBE
			{0x08, 0x54, 0x34, 0x14, 0x7c}, //я	0xBF


};



#endif /* FONT_H_ */
