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
    send_command(0, 0b0010);   // set to 4-bit operation
	_delay_ms(4);
    send_command_8(0, 0b00101000);   // function set: 4 bits, 2 lines, 5x11 font
	_delay_ms(4);
    send_command_8(0, 0b00001110);   // display/cursor on
	_delay_ms(4);
    send_command_8(0, 0b00000110);   // entry mode
	_delay_ms(4);

	display_set_value(0);
}

void display_set_value(int64_t new_value)
{
	if (new_value != value) {
		value = new_value;

		send_command_8(0, 0b00000001);   // clear screen
		_delay_ms(2);

		send_command_8(1, 'A');

		send_command_8(0, 0xc0);
		_delay_ms(2);

		send_command_8(1, 'B');

		/*
		send_command_8(0, 0b10000000 + 0x40);
		_delay_ms(2);
		*/

		/*
		char val[17];
		snprintf(val, 17, "%-16d", value);
		for (int i = 15; i >= 0; --i) {
			send_command_8(1, val[i]);
			_delay_us(40);
		}

		send_command_8(0, 0b10000000 + 40);

		snprintf(val, 17, "%-15Xh", value);
		for (int i = 15; i >= 0; --i) {
			send_command_8(1, val[i]);
			_delay_us(40);
		}
		*/
	}
}
