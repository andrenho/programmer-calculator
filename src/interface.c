#include "interface.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int64_t   reg = 0;
static int64_t   current = 0;
static int64_t   memory = 0;
static Mode      mode = DEC;
static bool      function = false;
static uint8_t   size = S_QWORD;
static bool      signed_ = true;
static Operation operation = O_NOP;
static bool      reset_display = false;

static const int64_t max_value = 999999999999999;

void interface_init(void)
{
    reg = 0;
    current = 0;
    memory = 0;
    mode = DEC;
    function = false;
    size = S_QWORD;
    signed_ = true;
    operation = O_NOP;
    reset_display = false;
}

static void add_digit(int8_t n)
{
	if (reset_display) {
		current = 0;
		reset_display = false;
	}

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
		case S_BYTE:
			if (new_value >= INT8_MIN && new_value <= INT8_MAX)
				current = new_value;
			break;
		case S_WORD:
			if (new_value >= INT16_MIN && new_value <= INT16_MAX)
				current = new_value;
			break;
		case S_DWORD:
			if (new_value >= INT32_MIN && new_value <= INT32_MAX)
				current = new_value;
			break;
		case S_QWORD:
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
		case S_QWORD: size = S_DWORD; break;
		case S_DWORD: size = S_WORD; break;
		case S_WORD:  size = S_BYTE; break;
		case S_BYTE:  size = S_QWORD; break;
	}
	current = 0;
}

static int64_t limit_value(int64_t value)
{
	switch (size) {
		case S_BYTE:  return value & 0xff;
		case S_WORD:  return value & 0xffff;
		case S_DWORD: return value & 0xffffffff;
		case S_QWORD: return value % max_value;
	}
    return 0;
}

static int64_t execute_last_operation(void)
{
	switch (operation) {
		case O_NOP:  return current;
		case O_ADD:  return reg + current;
		case O_SUB:  return reg - current;
		case O_MUL:  return reg * current;
		case O_DIV:  return reg / current;
		case O_AND:  return reg & current;
		case O_OR:   return reg | current;
		case O_XOR:  return reg ^ current;
		case O_MOD:  return reg % current;
		case O_LSH:  return reg << current;
		case O_RSH:  return reg >> current;
		case O_ROL:  return 0; // TODO
		case O_ROR:  return 0; // TODO
		case O_NAND: return ~(reg & current);
		case O_NOR:  return ~(reg | current);
		case O_XNOR: return ~(reg ^ current);
	}

    return current;
}

static void add_operation(Operation op)
{
	reg = limit_value(execute_last_operation());
	current = reg;
	operation = op;
	reset_display = true;
}

void interface_key_pressed(int8_t key)
{
	if (!function) {
		switch ((Key) key) {
			case K_0:      add_digit(0); break;
			case K_1:      add_digit(1); break;
			case K_2:      add_digit(2); break;
			case K_3:      add_digit(3); break;
			case K_4:      add_digit(4); break;
			case K_5:      add_digit(5); break;
			case K_6:      add_digit(6); break;
			case K_7:      add_digit(7); break;
			case K_8:      add_digit(8); break;
			case K_9:      add_digit(9); break;
			case K_A:      add_digit(0xa); break;
			case K_B:      add_digit(0xb); break;
			case K_C:      add_digit(0xc); break;
			case K_D:      add_digit(0xd); break;
			case K_E:      add_digit(0xe); break;
			case K_F:      add_digit(0xf); break;
			case K_BS:     backspace(); break;
			case K_SZ:     change_size(); break;
			case K_CLR:    current = 0; operation = O_NOP; break;
			case K_NOT:    current = ~current; break;
			case K_SIGN:   current = -current; break;
			case K_PLUS:   add_operation(O_ADD); break;
			case K_MINUS:  add_operation(O_SUB); break;
			case K_MUL:    add_operation(O_MUL); break;
			case K_DIV:    add_operation(O_DIV); break;
			case K_AND:    add_operation(O_AND); break;
			case K_OR:     add_operation(O_OR); break;
			case K_MOD:    add_operation(O_MOD); break;
			case K_XOR:    add_operation(O_XOR); break;
			case K_EQUALS: add_operation(O_NOP); break;
			case K_FUN:  function = !function; break;
			default: break;
		}
	} else if (function) {
		switch ((Key) key) {
			case K_M: memory += current; function = false; break;
			case K_MR: current = memory; function = false; break;
			case K_MC: memory= 0; function = false; break;
			case K_LSH:   add_operation(O_LSH); break;
			case K_RSH:   add_operation(O_RSH); break;
			case K_ROL:   add_operation(O_ROL); break;
			case K_ROR:   add_operation(O_ROR); break;
			case K_NAND:  add_operation(O_NAND); break;
			case K_NOR:   add_operation(O_NOR); break;
			case K_XNOR:  add_operation(O_XNOR); break;
			default: break;
		}
		function = false;
	}

	switch ((Key) key) {
		case K_MODE: change_mode(); break;
		default: break;
	}

}

void interface_display(char line[2][16])
{
	memset(line[0], ' ', 16);
	memset(line[1], ' ', 16);
	line[0][15] = '0';

	int64_t v = current % max_value;
    if (v < 0) v = -v;
	uint8_t pos = 15;
	while (v != 0) {
		int m = v % 10;
		line[0][pos--] = m + '0';
		v /= 10;
	}
	if (current < 0)
		line[0][pos] = '-';

	switch (size) {
		case S_BYTE:
			snprintf(&line[1][13], 8, "%2Xh", (uint8_t) current);
			break;
		case S_WORD:
			snprintf(&line[1][11], 6, "%4Xh", (uint16_t) current);
			break;
		case S_DWORD:
			snprintf(&line[1][7], 10, "%8Xh", (uint32_t) current);
			break;
		case S_QWORD:
			if ((current >> 32) != 0)
				snprintf(line[1], 8, " %6X", (uint32_t) (current >> 32));
			snprintf(&line[1][7], 10, "%8Xh", (uint32_t) current);
			break;
	}

	if (memory != 0)
		line[0][0] = 0b11101011;  // memory symbol

	switch (operation) {
        case O_NOP:  break;
		case O_ADD:  line[0][0] = '+'; break;
		case O_SUB:  line[0][0] = '-'; break;
		case O_MUL:  line[0][0] = 'x'; break;
		case O_DIV:  line[0][0] = 0b11111101; break;
		case O_AND:  line[0][0] = '&'; break;
		case O_OR:   line[0][0] = '|'; break;
		case O_XOR:  line[0][0] = '^'; break;
		case O_MOD:  line[0][0] = '%'; break;
		case O_LSH:  line[0][0] = '<'; break;
		case O_RSH:  line[0][0] = '>'; break;
		case O_ROL:  line[0][0] = 0b01111111; break;
		case O_ROR:  line[0][0] = 0b01111110; break;
		case O_NAND: line[0][0] = '!'; break;
		case O_NOR:  line[0][0] = 0b11001110; break;
		case O_XNOR: line[0][0] = 0b10110111; break;
	}

	if (function)
		line[0][0] = 'f';

	switch (size) {
		case S_BYTE: line[1][0] = 'b'; break;
		case S_WORD: line[1][0] = 'w'; break;
		case S_DWORD: line[1][0] = 'd'; break;
		case S_QWORD: line[1][0] = ' '; break;
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

Size interface_size(void)
{
	return size;
}
