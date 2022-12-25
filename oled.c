#include "stm32f4xx.h"									// Device header
#include "gpio.h"
#include "spi.h"
#include "string.h"
#include "oled.h"
#include "math.h"
#include "tools.h"

#define SSD1327_BUFFERSIZE	(SSD1327_LCDHEIGHT * SSD1327_LCDWIDTH / 2)

static uint8_t buffer[SSD1327_BUFFERSIZE];
static uint8_t cmd[2];


void SSD1327_Command(uint8_t *com)
{
	gpio_down(GPIO_PIN_OLED_DC);
	gpio_down(GPIO_PIN_OLED_CS);

	spi_transmit(com, 1);

	gpio_up(GPIO_PIN_OLED_CS);
}

void SSD1327_Data(uint8_t dat)
{
	gpio_up(GPIO_PIN_OLED_DC);
	gpio_down(GPIO_PIN_OLED_CS);

	spi_transmit(&dat, 1);

	gpio_up(GPIO_PIN_OLED_CS);

}

void SSD1327_Reset(void)
{
	gpio_down(GPIO_PIN_OLED_RST);
	delay_nops(5*DLMUL);
	gpio_up(GPIO_PIN_OLED_RST);
}


//
// Configuration functions
//
void SSD1327_InvertColors(uint8_t Invert)
{
	cmd[0] = Invert ? SSD1327_INVERTDISPLAY : SSD1327_NORMALDISPLAY;
	SSD1327_Command(cmd);
}

void SSD1327_RotateDisplay(uint8_t Rotate)
{
	if(Rotate > 1) Rotate = 1;
	
	cmd[0] =0xA0 | (0x01 & Rotate);
	SSD1327_Command(cmd);	// Set Segment Re-Map Default
                        // 0xA0 (0x00) => column Address 0 mapped to 127
                        // 0xA1 (0x01) => Column Address 127 mapped to 0
	cmd[0] = 0xC0 | (0x08 & (Rotate<<3));
	SSD1327_Command(cmd);	// Set COM Output Scan Direction
                        // 0xC0	(0x00) => normal mode (RESET) Scan from COM0 to COM[N-1];Where N is the Multiplex ratio.
                        // 0xC8	(0xC8) => remapped mode. Scan from COM[N-1] to COM0;;Where N is the Multiplex ratio.
}

void SSD1327_DisplayON(uint8_t On)
{
	cmd[0] = On ? SSD1327_DISPLAYON : SSD1327_DISPLAYOFF;
	SSD1327_Command(cmd);
}

void SSD1327_SetContrast(uint8_t Contrast)
{
	cmd[0] = SSD1327_SETCONTRASTCURRENT; SSD1327_Command(cmd);	// Set Contrast Control
	cmd[0] = Contrast; SSD1327_Command(cmd);
	
}


//
// Initialization
//
void SSD1327_Init(void)
{
  cmd[0] = 0x0ae;SSD1327_Command(cmd); //--turn off oled panel
  cmd[0] = 0x015;SSD1327_Command(cmd);
  cmd[0] = 0x000;SSD1327_Command(cmd);
  cmd[0] = 0x07f;SSD1327_Command(cmd);		//set column address, start column 0, end column 127
  
  cmd[0] = 0x075;SSD1327_Command(cmd);
  cmd[0] = 0x000;SSD1327_Command(cmd);
  cmd[0] = 0x07f;SSD1327_Command(cmd);		//set row address, start row 0, end row 127
  
  cmd[0] = 0x081;SSD1327_Command(cmd);
  cmd[0] = 0x080;SSD1327_Command(cmd);		//set contrast control
  cmd[0] = 0x0a0;SSD1327_Command(cmd);
  cmd[0] = 0x051;SSD1327_Command(cmd);		//gment remap, 51
  cmd[0] = 0x0a1;SSD1327_Command(cmd);
  cmd[0] = 0x000;SSD1327_Command(cmd);		//start line
  cmd[0] = 0x0a2;SSD1327_Command(cmd);
  cmd[0] = 0x000;SSD1327_Command(cmd);		//display offset
  cmd[0] = 0x0a4;SSD1327_Command(cmd);
  cmd[0] = 0x0a8;SSD1327_Command(cmd);
  cmd[0] = 0x07f;SSD1327_Command(cmd);		//rmal display, set multiplex ratio
  cmd[0] = 0x0b1;SSD1327_Command(cmd);
  cmd[0] = 0x0f1;SSD1327_Command(cmd);		//set phase leghth
  cmd[0] = 0x0b3;SSD1327_Command(cmd);
  cmd[0] = 0x000;SSD1327_Command(cmd);		//set dclk, 80Hz:0xc1 90Hz:0xe1	 100Hz:0x00	 110Hz:0x30 120Hz:0x50	 130Hz:0x70		 01
  cmd[0] = 0x0ab;SSD1327_Command(cmd);
  cmd[0] = 0x001;SSD1327_Command(cmd);		//
  cmd[0] = 0x0b6;SSD1327_Command(cmd);
  cmd[0] = 0x00f;SSD1327_Command(cmd);		//set phase leghth
  cmd[0] = 0x0be;SSD1327_Command(cmd);
  cmd[0] = 0x00f;SSD1327_Command(cmd);
  cmd[0] = 0x0bc;SSD1327_Command(cmd);
  cmd[0] = 0x008;SSD1327_Command(cmd);
  cmd[0] = 0x0d5;SSD1327_Command(cmd);
  cmd[0] = 0x062;SSD1327_Command(cmd);
  cmd[0] = 0x0fd;SSD1327_Command(cmd);
  cmd[0] = 0x012;SSD1327_Command(cmd);
//cmd[0] = SSD1327_NORMALDISPLAY;SSD1327_Command(cmd);	// Set Normal Display

  cmd[0] = SSD1327_DISPLAYALLON_RESUME;SSD1327_Command(cmd);	// Entire Display ON
  SSD1327_DisplayON(1);
}


void SSD1327_SpiInit(void)
{
  gpio_init(GPIO_PIN_OLED_RST);
  gpio_init(GPIO_PIN_OLED_CS);
  gpio_init(GPIO_PIN_OLED_DC);

	SSD1327_Reset();

	SSD1327_Init();
}


//
// Draw pixel in the buffer
//
void SSD1327_DrawPixel(int16_t x, int16_t y, uint8_t Color)
{
	 uint8_t SelectedCell = buffer[x/2 + y*(SSD1327_LCDWIDTH/2)];

	 if(x % 2)
	 {
		 SelectedCell &= ~(0x0F);
		 SelectedCell |= (0x0F & Color);
	 }
	 else
	 {
		 SelectedCell &= ~(0xF0);
		 SelectedCell |= (0xF0 & (Color<<4));
	 }

	 buffer[x/2 + y*(SSD1327_LCDWIDTH/2)] = SelectedCell;
}

//
// Clear the buffer
//
void SSD1327_Clear(uint8_t Color)
{
	if(Color > WHITE) Color = WHITE;

	memset(buffer, (Color << 4 | Color), SSD1327_BUFFERSIZE);
}

//
// Send buffer to OLDE GCRAM
//
void SSD1327_Display(void)
{

  gpio_up(GPIO_PIN_OLED_DC);

	gpio_down(GPIO_PIN_OLED_CS);

	spi_transmit((uint8_t*)&buffer, SSD1327_BUFFERSIZE);

	gpio_up(GPIO_PIN_OLED_CS);

}

//
// Display Bitmap directly on screen
//
void SSD1327_Bitmap(uint8_t *bitmap)
{
	cmd[0] = 0x22;SSD1327_Command(cmd);
	cmd[0] = 0x00;SSD1327_Command(cmd);
	cmd[0] = 0x07;SSD1327_Command(cmd);

	gpio_up(GPIO_PIN_OLED_DC);

	gpio_down(GPIO_PIN_OLED_CS);

	spi_transmit(bitmap, (SSD1327_LCDHEIGHT * SSD1327_LCDWIDTH / 8));

	gpio_up(GPIO_PIN_OLED_CS);
}


