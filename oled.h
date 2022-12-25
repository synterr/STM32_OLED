#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define USE_SPI_4W 1

#define OLED_1in5_WIDTH  128 //OLED width
#define OLED_1in5_HEIGHT 128 //OLED height

//
// Resolution
//
#define SSD1327_LCDWIDTH                  128
#define SSD1327_LCDHEIGHT                 128


//
// Commands
//
#define SSD1327_SETCOLUMNADDRESS	0x15
#define SSD1327_SETROWADDRESS		0x75
#define SSD1327_SETCONTRASTCURRENT 	0x81
#define SSD1327_NOP					0x84
#define SSD1327_SEGREMAP			0xA0
#define SSD1327_SETDISPLAYSTARTLINE	0xA1
#define SSD1327_SETDISPLAYOFFSET	0xA2
#define SSD1327_DISPLAYALLON_RESUME 0xA4
#define SSD1327_DISPLAYALLON 		0xA5
#define SSD1327_NORMALDISPLAY 		0xA6
#define SSD1327_INVERTDISPLAY 		0xA7
#define SSD1327_SETMULTIPLEX		0xA8
#define SSD1327_FUNCTIONSELECTIONA	0xAB
#define SSD1327_DISPLAYOFF			0xAE
#define SSD1327_DISPLAYON			0xAF
#define SSD1327_SETPHASELENGTH		0xB1
#define SSD1327_SETFRONTCLOCKDIVIDER_OSCILLATORFREQUENCY 0xB3
#define SSD1327_SETGPIO				0xB5
#define SSD1327_SETSECONDPRECHARGEPERTIOD	0xB6
#define SSD1327_SETGRAYSCALETABLE	0xB8
#define SSD1327_SELECTDEFAULTLINEARGRAYSCALETABLE	0xB9
#define SSD1327_SETPRECHARGEVOLTAGE	0xBC
#define SSD1327_SETSETVCOMVOLTAGE	0xBE
#define SSD1327_FUNCTIONSELECTIONB	0xD5
#define SSD1327_SETCOMMANDLOCK		0xFD

//
// Colors
//
#define BLACK 0
// Grays between
#define WHITE 15

void SSD1327_Init(void);
void SSD1327_SpiInit(void);
//
// Configuration
//
void SSD1327_DisplayON(uint8_t On);
void SSD1327_InvertColors(uint8_t Invert);
void SSD1327_RotateDisplay(uint8_t Rotate);
void SSD1327_SetContrast(uint8_t Contrast);

void SSD1327_Command(uint8_t *com);
void SSD1327_Data(uint8_t dat);
void SSD1327_Reset(void);


//
// Drawing
//
void SSD1327_DrawPixel(int16_t x, int16_t y, uint8_t Color);
void SSD1327_Clear(uint8_t Color);
void SSD1327_Display(void);
void SSD1327_Bitmap(uint8_t *bitmap);


#endif

