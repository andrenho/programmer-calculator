#include "interface.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int64_t reg = 0;
static int64_t current = 0;
static int64_t memory = 0;
static Mode    mode = DEC;
static bool    function = false;
static uint8_t size = 8;

static const int64_t max_value = 999999999999999;

void interface_init(void)
{
}

static void add_digit(int8_t n)
{
	int64_t new_value = current;

	switch (mode) {
		case DEC:
			if (n <= 9)
				new_value = (current * 10) + n;
			break;
		case HEX:
			new_value = (current * 0x10) + n;
			break;
		case BIN:
			if (n <= 1)
				new_value = (current * 0b10) + n;
			break;
	}

	switch (size) {
		case 1:
			if (new_value >= INT8_MIN && new_value <= INT8_MAX)
				current = new_value;
			break;
		case 2:
			if (new_value >= INT16_MIN && new_value <= INT16_MAX)
				current = new_value;
			break;
		case 4:
			if (new_value >= INT32_MIN && new_value <= INT32_MAX)
				current = new_value;
			break;
		case 8:
			if (new_value >= -max_value && new_value <= max_value)
				current = new_value;
			break;
	}
}

static void backspace(void)
{
	switch (mode) {
		case DEC: current /= 10; break;
		case HEX: current /= 0x10; break;
		case BIN: current /= 0b10; break;
	}
}

static void change_mode(void)
{
	mode += 1;
	if (mode > BIN)
		mode = DEC;
	current = 0;
}

static void change_size(void)
{
	switch (size) {
		case 8: size = 4; break;
		case 4: size = 2; break;
		case 2: size = 1; break;
		case 1: size = 8; break;
	}
	current = 0;
}

void interface_key_pressed(int8_t key)
{
	if (!function) {
		switch ((Key) key) {
			case K_0:    add_digit(0); break;
			case K_1:    add_digit(1); break;
			case K_2:    add_digit(2); break;
			case K_3:    add_digit(3); break;
			case K_4:    add_digit(4); break;
			case K_5:    add_digit(5); break;
			case K_6:    add_digit(6); break;
			case K_7:    add_digit(7); break;
			case K_8:    add_digit(8); break;
			case K_9:    add_digit(9); break;
			case K_A:    add_digit(0xa); break;
			case K_B:    add_digit(0xb); break;
			case K_C:    add_digit(0xc); break;
			case K_D:    add_digit(0xd); break;
			case K_E:    add_digit(0xe); break;
			case K_F:    add_digit(0xf); break;
			case K_BS:   backspace(); break;
			case K_SZ:   change_size(); break;
			case K_CLR:  current = 0; break;
			case K_NOT:  current = ~current; break;
			case K_SIGN: current = -current; break;
		}
	} else if (function) {
		switch ((Key) key) {
			case K_M: memory += current; function = false; break;
			case K_MR: current = memory; function = false; break;
			case K_MC: memory= 0; function = false; break;
		}
	}

	switch ((Key) key) {
		case K_FUN:  function = !function; break;
		case K_MODE: change_mode(); break;
	}

}

void interface_display(char line[2][16])
{
	memset(line[0], ' ', 16);
	memset(line[1], ' ', 16);
	line[0][15] = '0';

	int64_t v = abs(current % max_value);
	uint8_t pos = 15;
	while (v != 0) {
		int m = v % 10;
		line[0][pos--] = m + '0';
		v /= 10;
	}
	if (current < 0)
		line[0][pos] = '-';

	switch (size) {
		case 1:
			snprintf(&line[1][13], 8, "%2Xh", (uint8_t) current);
			break;
		case 2:
			snprintf(&line[1][11], 6, "%4Xh", (uint16_t) current);
			break;
		case 4:
			snprintf(&line[1][7], 10, "%8lXh", (uint32_t) current);
			break;
		case 8:
			if ((current >> 32) != 0)
				snprintf(line[1], 8, " %6lX", (uint32_t) (current >> 32));
			snprintf(&line[1][7], 10, "%8lXh", (uint32_t) current);
			break;
	}

	if (memory != 0)
		line[0][0] = 0b11101011;  // memory symbol

	if (function)
		line[0][0] = 'f';

	switch (size) {
		case 1: line[1][0] = 'b'; break;
		case 2: line[1][0] = 'w'; break;
		case 4: line[1][0] = 'd'; break;
		case 8: line[1][0] = ' '; break;
	}
}

int64_t interface_value(void)
{
	return current;
}

Mode interface_mode(void)
{
	return mode;
}

uint8_t interface_size(void)
{
	return size;
}
