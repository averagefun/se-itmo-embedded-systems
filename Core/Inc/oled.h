#include "stm32f4xx_hal.h"
#include "fonts.h"

#ifndef OLED_H
#define OLED_H

#define OLED_I2C_ADDR        0x78
#define OLED_WIDTH           128
#define OLED_HEIGHT          64
#define OLED_STRING_SPACE    3

typedef enum {
	BLACK = 0x00,
	WHITE = 0x01
} OLED_COLOR;

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
} OLED_State;

extern I2C_HandleTypeDef OLED_I2C_PORT;

uint8_t oled_Init(void);
void oled_Fill(OLED_COLOR color);
void oled_UpdateScreen(void);
void oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color);
char oled_WriteChar(char ch, FontType Font, OLED_COLOR color);
char oled_WriteString(char* str, FontType Font, OLED_COLOR color);
void oled_SetCursor(uint8_t x, uint8_t y);
void oled_NextLine();
void oled_Reset();

#endif
