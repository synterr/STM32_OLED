#include <stdlib.h>
#include "stm32f4xx.h"                  // Device header
#include "Cellular.h"
#include "oled.h"

void InitCells(uint16_t rnd)
{
  if(rnd > 0)
  {
    for (uint8_t y = 0; y < BOARD_SIZE; y++)
    {
      for (uint8_t x = 0; x < BOARD_SIZE; x++)
      {
        srand(x+y+rand()+rnd);
        cell[x][y] = 0;
        old_cell[x][y] = rand()%2;
        
      }
    }
  }
  else
  {
   for (uint8_t y = 0; y < BOARD_SIZE; y++)
    {
      for (uint8_t x = 0; x < BOARD_SIZE; x++)
      {
        cell[x][y] = 0;
        old_cell[x][y] = 0;
      }
    }
  }
}

void SwapCells(void)
{
 
  memcpy(old_cell, cell, sizeof(cell));
}

void CalcCells(void)
{
  
  for (int16_t y = 0; y < BOARD_SIZE; y++)
  {
    for (int16_t x = 0; x < BOARD_SIZE; x++)
    {
      uint8_t neighbors = 0;
      for (int16_t i = -1; i <= 1; i++) {
        for (int16_t j = -1; j <= 1; j++) {
          int16_t a = x+i;
          int16_t b = y+j;

          if (a < 0) a = BOARD_SIZE-1;
          if (b < 0) b = BOARD_SIZE-1;
          if (a > BOARD_SIZE - 1) a = 0;
          if (b > BOARD_SIZE - 1) b = 0;
          
          neighbors += old_cell[a][b];
        }
      }

      // A little trick to subtract the current cell's state since
      // we added it in the above loop
      neighbors -= old_cell[x][y];
      // Rules of Life
      if      ((old_cell[x][y] == 1) && (neighbors <  2)){ cell[x][y] = 0;}           // Loneliness
      else if ((old_cell[x][y] == 1) && (neighbors >  3)){ cell[x][y] = 0;}          // Overpopulation
      else if ((old_cell[x][y] == 0) && (neighbors == 3)){ cell[x][y] = 1;}          // Reproduction
      else     cell[x][y] = old_cell[x][y]; // Stasis    
      
      if (cell[x][y] != old_cell[x][y])
      {
        SSD1327_DrawPixel(x*2,   y*2,   cell[x][y] * WHITE);
        SSD1327_DrawPixel(x*2+1, y*2,   cell[x][y] * WHITE);
        SSD1327_DrawPixel(x*2,   y*2+1, cell[x][y] * WHITE);
        SSD1327_DrawPixel(x*2+1, y*2+1, cell[x][y] * WHITE);
      }
      
    }
  }
}
