#include "oled.h"
#include "i2c.h"

static uint8_t OLED_Buffer[1024];

static OLED_State OLED;


static void oled_WriteCommand(uint8_t command) {
	HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x00,1,&command,1,10);
}


uint8_t oled_Init(void) {
	HAL_Delay(100);

	oled_WriteCommand(0xAE);
	oled_WriteCommand(0x20);
	oled_WriteCommand(0x10);
	oled_WriteCommand(0xB0);
	oled_WriteCommand(0xC8);
	oled_WriteCommand(0x00);
	oled_WriteCommand(0x10);
	oled_WriteCommand(0x40);
	oled_WriteCommand(0x81);
	oled_WriteCommand(0xFF);
	oled_WriteCommand(0xA1);
	oled_WriteCommand(0xA6);
	oled_WriteCommand(0xA8);
	oled_WriteCommand(0x3F);
	oled_WriteCommand(0xA4);
	oled_WriteCommand(0xD3);
	oled_WriteCommand(0x00);
	oled_WriteCommand(0xD5);
	oled_WriteCommand(0xF0);
	oled_WriteCommand(0xD9);
	oled_WriteCommand(0x22);
	oled_WriteCommand(0xDA);
	oled_WriteCommand(0x12);
	oled_WriteCommand(0xDB);
	oled_WriteCommand(0x20);
	oled_WriteCommand(0x8D);
	oled_WriteCommand(0x14);
	oled_WriteCommand(0xAF);

	oled_Fill(BLACK);

	oled_UpdateScreen();

	OLED.CurrentX = 0;
	OLED.CurrentY = 0;


	return 0;
}

void oled_Fill(OLED_COLOR color) {
	for(uint32_t i = 0; i < sizeof(OLED_Buffer); i++) {
		OLED_Buffer[i] = (color == BLACK) ? 0x00 : 0xFF;
	}
}


void oled_UpdateScreen(void) {
	for (uint8_t i = 0; i < 8; i++) {
		oled_WriteCommand(0xB0 + i);
		oled_WriteCommand(0x00);
		oled_WriteCommand(0x10);

		HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x40,1,&OLED_Buffer[OLED_WIDTH * i],OLED_WIDTH,100);
	}
}

void oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color) {
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT) {
		return;
	}

	if (color == WHITE) {
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	} else {
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
}

char oled_WriteChar(char ch, FontType Font, OLED_COLOR color) {
	uint32_t i, b, j;

	if (OLED_WIDTH <= (OLED.CurrentX + Font.FontWidth)) {
		if(OLED_HEIGHT <= (OLED.CurrentY + Font.FontHeight*2 + OLED_STRING_SPACE)) {
			return 0;
		}
		oled_SetCursor(0, OLED.CurrentY + Font.FontHeight + OLED_STRING_SPACE);
	}

	for (i = 0; i < Font.FontHeight; i++) {
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++) {
			if ((b << j) & 0x8000) {
				oled_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR) color);
			} else {
				oled_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR)!color);
			}
		}
	}

	oled_SetCursor(OLED.CurrentX + Font.FontWidth, OLED.CurrentY);

	return ch;
}


char oled_WriteString(char* str, FontType Font, OLED_COLOR color) {
	while (*str) {
		if (oled_WriteChar(*str, Font, color) != *str) {
			return *str;
		}
		str++;
	}
	return *str;
}

void oled_Reset() {
	oled_Fill(BLACK);
	oled_SetCursor(0,0);
}

void oled_SetCursor(uint8_t x, uint8_t y) {
	OLED.CurrentX = x;
	OLED.CurrentY = y;
}

void oled_NextLine(FontType Font) {
	if(OLED_HEIGHT <= (OLED.CurrentY + Font.FontHeight*2 + OLED_STRING_SPACE)) {
		return;
	}
	oled_SetCursor(0, OLED.CurrentY + Font.FontHeight + OLED_STRING_SPACE);
}

//
//#include "oled.h"
//
//static OLED_state OLED;
//
//static uint8_t OLED_buffer[OLED_WIDTH * OLED_HEIGHT / 8];
//
//static void OLED_write_command(uint8_t command) {
//	HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x00,1,&command,1,10);
//}
//
//void OLED_init(void) {
//	HAL_Delay(100);
//
//	OLED_write_command(0xAE);
//	OLED_write_command(0x20);
//	OLED_write_command(0x10);
//	OLED_write_command(0xB0);
//	OLED_write_command(0xC8);
//	OLED_write_command(0x00);
//	OLED_write_command(0x10);
//	OLED_write_command(0x40);
//	OLED_write_command(0x81);
//	OLED_write_command(0xFF);
//	OLED_write_command(0xA1);
//	OLED_write_command(0xA6);
//	OLED_write_command(0xA8);
//	OLED_write_command(0x3F);
//	OLED_write_command(0xA4);
//	OLED_write_command(0xD3);
//	OLED_write_command(0x00);
//	OLED_write_command(0xD5);
//	OLED_write_command(0xF0);
//	OLED_write_command(0xD9);
//	OLED_write_command(0x22);
//	OLED_write_command(0xDA);
//	OLED_write_command(0x12);
//	OLED_write_command(0xDB);
//	OLED_write_command(0x20);
//	OLED_write_command(0x8D);
//	OLED_write_command(0x14);
//	OLED_write_command(0xAF);
//
//	OLED_fill(BLACK);
//
//	OLED_set_cursor(0,0);
//}
//
//void OLED_update(void) {
//	for (uint8_t i = 0; i < 8; i++) {
//			OLED_write_command(0xB0 + i);
//			OLED_write_command(0x00);
//			OLED_write_command(0x10);
//
//			HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x40,1,&OLED_buffer[OLED_WIDTH * i],OLED_WIDTH,100);
//		}
//}
//
//
//void OLED_set_cursor(uint16_t x, uint16_t y) {
//	OLED.curr_x = x;
//	OLED.curr_y = y;
//}
//
//void OLED_draw_pixel(uint16_t x, uint16_t y, OLED_color color) {
//	if (x >= OLED_WIDTH || y >= OLED_HEIGHT) {
//			return;
//	}
//
//	switch(color) {
//	case WHITE:
//		OLED_buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
//		break;
//	case BLACK:
//		OLED_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
//		break;
//	}
//}
//
//void OLED_fill(OLED_color color) {
//	uint32_t i;
//	for(i = 0; i < sizeof(OLED_buffer); i++) {
//		OLED_buffer[i] = (color == BLACK) ? 0x00 : 0xFF;
//	}
//}
//
//void OLED_draw_char(const char ch, font font, OLED_color color) {
//	if (OLED_WIDTH <= (OLED.curr_x + font.font_width)) {
//		if(OLED_HEIGHT <= (OLED.curr_y + font.font_height*2 + OLED_STRING_VSPACE)) {
//			return;
//		}
//		OLED_set_cursor(0, OLED.curr_y + OLED_STRING_VSPACE + font.font_height);
//	}
//
//	for (uint32_t i = 0; i < font.font_height; i++) {
//		uint32_t b = font.data[(ch - 32) * font.font_height + i];
//		for (uint32_t j = 0; j < font.font_width; j++) {
//			if ((b << j) & 0x8000) {
//				OLED_draw_pixel(OLED.curr_x + j, (OLED.curr_y + i), color);
//			} else {
//				OLED_draw_pixel(OLED.curr_x + j, (OLED.curr_y + i), !color);
//			}
//		}
//	}
//
//	OLED_set_cursor(OLED.curr_x + font.font_width, OLED.curr_y);
//}
//
//
//void OLED_draw_string(char const* str, font font, OLED_color color) {
//	while(!str) {
//		OLED_draw_char(*str, font, color);
//		str++;
//	}
//}
