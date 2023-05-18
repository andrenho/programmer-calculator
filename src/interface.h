#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <stdint.h>

void interface_init(void);
void interface_key_pressed(int8_t key);
void interface_display(char line[2][16]);
int64_t interface_value(void);

#endif
