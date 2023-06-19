#ifndef INC_KEYBOARD_H
#define INC_KEYBOARD_

#define ROWS_LENGTH 4
#define ROW1 0xFE
#define ROW2 0xFD
#define ROW3 0xFB
#define ROW4 0xF7
static uint8_t rows_config[ROWS_LENGTH] = {ROW1, ROW2, ROW3, ROW4};

#define COLS_LENGTH 3
#define COL1 0x10
#define COL2 0x20
#define COL3 0x40
static uint8_t cols[COLS_LENGTH] = {COL1, COL2, COL3};

uint8_t Scan_KB();


#endif
