#include "interface.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SYMBOL   0b11101011
#define DIVISOIN_SYMBOL 0b11111101
#define ROL_SYMBOL      0b01111111
#define ROR_SYMBOL      0b01111110
#define NOR_SYMBOL      0b11001110
#define XNOR_SYMBOL     0b10110111

typedef enum Operation {
    O_NOP,
    O_ADD,
    O_SUB,
    O_MUL,
    O_DIV,
    O_AND,
    O_OR,
    O_XOR,
    O_MOD,
    O_LSH,
    O_RSH,
    O_ROL,
    O_ROR,
    O_NAND,
    O_NOR,
    O_XNOR,
} Operation;

static int64_t   value = 0;
static int64_t   memory = 0;
static Mode      mode = DEC;
static bool      function = false;
static uint8_t   size = S_UNLIMITED;
static Operation operation = O_NOP;

static const int64_t max_value = 999999999999999;

void interface_init(void)
{
}

void interface_key_pressed(int8_t key)
{
    (void) key;
}

void interface_display(char line[2][16])
{
    // clean up
    memset(line[0], ' ', 16);
    memset(line[1], ' ', 16);

    // build first line
    line[0][15] = '0';
    int64_t v = value % max_value;
    if (v < 0) v = -v;
    uint8_t pos = 15;
    while (v != 0) {
        int m = v % 10;
        line[0][pos--] = m + '0';
        v /= 10;
    }
    if (value < 0)
        line[0][pos] = '-';   // is negative?

    // build second line
    switch (size) {
        case S_BYTE:
            snprintf(&line[1][13], 8, "%2Xh", (uint8_t) value);
            break;
        case S_WORD:
            snprintf(&line[1][11], 6, "%4Xh", (uint16_t) value);
            break;
        case S_DWORD:
            snprintf(&line[1][7], 10, "%8Xh", (uint32_t) value);
            break;
        case S_UNLIMITED:
            if ((value >> 32) != 0)
                snprintf(line[1], 8, " %6X", (uint32_t) (value >> 32));
            snprintf(&line[1][7], 10, "%8Xh", (uint32_t) value);
            break;
    }

    // memory symbol
    if (memory != 0)
        line[0][0] = MEMORY_SYMBOL;  // memory symbol

    // current operation symbol
    switch (operation) {
        case O_NOP:  break;
        case O_ADD:  line[0][0] = '+'; break;
        case O_SUB:  line[0][0] = '-'; break;
        case O_MUL:  line[0][0] = 'x'; break;
        case O_DIV:  line[0][0] = DIVISOIN_SYMBOL; break;
        case O_AND:  line[0][0] = '&'; break;
        case O_OR:   line[0][0] = '|'; break;
        case O_XOR:  line[0][0] = '^'; break;
        case O_MOD:  line[0][0] = '%'; break;
        case O_LSH:  line[0][0] = '<'; break;
        case O_RSH:  line[0][0] = '>'; break;
        case O_ROL:  line[0][0] = ROL_SYMBOL; break;
        case O_ROR:  line[0][0] = ROR_SYMBOL; break;
        case O_NAND: line[0][0] = '!'; break;
        case O_NOR:  line[0][0] = NOR_SYMBOL; break;
        case O_XNOR: line[0][0] = XNOR_SYMBOL; break;
    }

    // 2nd function?
    if (function)
        line[0][0] = 'f';

    // show size
    switch (size) {
        case S_BYTE:      line[1][0] = 'b'; break;
        case S_WORD:      line[1][0] = 'w'; break;
        case S_DWORD:     line[1][0] = 'd'; break;
        case S_UNLIMITED: line[1][0] = ' '; break;
    }
}

int64_t interface_value(void)
{
    return value;
}

Mode interface_mode(void)
{
    return DEC; // TODO
}

Size interface_size(void)
{
    return S_UNLIMITED;
}