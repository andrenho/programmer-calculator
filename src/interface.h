#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <stdint.h>

#include "mode.h"

typedef enum Key {
    K_PLUS	 = 0,
    K_7		 = 1,
    K_8		 = 2,
    K_9		 = 3,
    K_MINUS	 = 4,
    K_4      = 5,
    K_5      = 6,
    K_6      = 7,
    K_MUL    = 8,
    K_1      = 9,
    K_2      = 10,
    K_3      = 11,
    K_DIV    = 12,
    K_SIGN   = 13,
    K_0      = 14,
    K_EQUALS = 15,

    // function keys (F1)
    K_BS     = 16,
    K_NOT	 = 17,
    K_AND    = 18,
    K_CLR    = 19,
    K_A      = 20,
    K_D      = 21,
    K_OR     = 22,
    K_MOD	 = 23,
    K_B      = 24,
    K_E      = 25,
    K_XOR    = 26,
    K_FUN    = 27,
    K_C      = 28,
    K_F      = 29,
    K_SZ     = 30,
    K_MODE   = 31,

    // function keys (F2)
    K_M      = 16,
    K_LSH	 = 17,
    K_NAND   = 18,
    K_MR     = 20,
    K_RSH    = 21,
    K_NOR    = 22,
    K_MC     = 24,
    K_ROL    = 25,
    K_XNOR   = 26,
    K_ROR    = 29,
} Key;

typedef enum Size {
    S_BYTE, S_WORD, S_DWORD, S_UNLIMITED,
} Size;

void    interface_init(void);
void    interface_key_pressed(int8_t key);
void    interface_display(char line[2][16]);
int64_t interface_value(void);
Mode    interface_mode(void);
Size    interface_size(void);

#endif
