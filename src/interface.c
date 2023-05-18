#include "interface.h"

#include <string.h>

void interface_init(void)
{
}

void interface_key_pressed(int8_t key)
{
	(void) key;
}

void interface_display(char line[2][16])
{
	strcpy(line[0], "0123456789012345");
	strcpy(line[1], "ABCDEFGHIJKLMNOP");
}

int64_t interface_value(void)
{
	return 0xff;
}
