#ifndef ASTPARSER_CALC_H
#define ASTPARSER_CALC_H

#include <stddef.h>
#include <stdint.h>

#define LAYOUTS_N 2
#define LAYOUT_WIDTH 3
#define LAYOUT_HEIGHT 4
#define LAYOUT_DEFAULT 0
#define CALC_BUF_SIZE 128

struct calculator_i {
    size_t cur;
    size_t last;
    char buf[CALC_BUF_SIZE];
    size_t layout;
};

typedef void (calc_Action)(struct calculator_i *calc);

void calc_Call_Action(struct calculator_i *calc, uint8_t index);

#endif
