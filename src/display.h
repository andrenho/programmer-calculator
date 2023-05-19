#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

void display_init(void);
void display_set_value(int64_t value);
void display_set_lines(const char lines[2][16]);

#endif
