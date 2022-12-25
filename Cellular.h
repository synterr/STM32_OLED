#ifndef CELLULAR_H
#define CELLULAR_H

#include <stdint.h>
#include <stdio.h>
#include "tools.h"

#define BOARD_SIZE 64

static uint8_t cell[BOARD_SIZE][BOARD_SIZE];
static uint8_t old_cell[BOARD_SIZE][BOARD_SIZE];

void InitCells(uint16_t rnd);
void SwapCells(void);
void CalcCells(void);
  
#endif

