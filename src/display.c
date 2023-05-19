#include "display.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#define set_E()    PORTD |= _BV(PD2)
#define clear_E()  PORTD &= ~_BV(PD2)
#define set_RS()   PORTD |= _BV(PD3)
#define clear_RS() PORTD &= ~_BV(PD3)
#define set_DATA(value) PORTD = (PORTD & 0xf) | ((value) << 4)

static int64_t value = -1;

static void send_command(bool rs, uint8_t data)
{
    if (rs) set_RS(); else clear_RS();
    set_DATA(data);
    set_E();
    clear_E();
}

static void send_command_8(bool rs, uint8_t data)
{
	send_command(rs, data >> 4);
	send_command(rs, data & 0xf);
	_delay_us(40);
}

void display_init(void)
{
    DDRD |= 0b11111100;
    clear_RS();
    clear_E();
    set_DATA(0);

    // initialize LCD
	for (int i = 0; i < 4; ++i) {
		send_command(0, 0b0010);   // set to 4-bit operation
		_delay_ms(4);
	}

    send_command_8(0, 0b00101000);   // function set: 4 bits, 2 lines, 5x11 font
	_delay_ms(4);
    send_command_8(0, 0b00001100);   // display/cursor on
	_delay_ms(4);
    send_command_8(0, 0b00000110);   // entry mode
	_delay_ms(4);
}


void display_set_lines(const char lines[2][16])
{
	send_command_8(0, 0b10000000);
	for (uint8_t col = 0; col < 16; ++col)
		send_command_8(1, lines[0][col]);

	send_command_8(0, 0b10000000 + 0x40);
	for (uint8_t col = 0; col < 16; ++col)
		send_command_8(1, lines[1][col]);
}
