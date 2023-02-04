
#include "stm32f4xx_hal.h"

#ifndef FONTS_H
#define FONTS_H

typedef struct {
	const uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} FontType;

extern FontType Font_7x10;

#endif
