#include "keyboard.h"

#include <stdbool.h>
#include <avr/cpufunc.h>
#include <avr/io.h>

#include "util/delay.h"

void keyboard_init(void)
{
	DDRA  = 0b11110000;
	DDRC  = 0b11110000;
	PORTA = 0b11111111;
	PORTC = 0b11111111;
}

int16_t keyboard_key_pressed(void)
{
	for (uint8_t column = 0; column < 4; ++column) {
		PORTC = ~(1 << (column + 4));
		_delay_us(4);
		uint8_t pinc = PINC;

		if ((pinc & (1 << 3)) == 0)
			return (column * 4);
		else if ((pinc & (1 << 2)) == 0)
			return (column * 4) + 1;
		else if ((pinc & (1 << 1)) == 0)
			return (column * 4) + 2;
		else if ((pinc & 1) == 0)
			return (column * 4) + 3;
	}

	for (uint8_t column = 0; column < 4; ++column) {
		PORTA = ~(1 << (column + 4));
		_delay_us(4);
		uint8_t pina = PINA;

		if ((pina & (1 << 3)) == 0)
			return (column * 4) + 16;
		else if ((pina & (1 << 2)) == 0)
			return (column * 4) + 17;
		else if ((pina & (1 << 1)) == 0)
			return (column * 4) + 18;
		else if ((pina & 1) == 0)
			return (column * 4) + 19;
	}

	return -1;
}
