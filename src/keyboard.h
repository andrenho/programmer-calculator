#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

void    keyboard_init(void);
int16_t keyboard_key_pressed(void);

/* Keys:
 *
 *  00 01 02 03    16 20 24 28
 *  04 05 06 07    17 21 25 29
 *  08 09 10 11    18 22 26 30
 *  12 13 14 15    19 23 27 31 */

#endif
