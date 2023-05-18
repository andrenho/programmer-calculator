#include "leds.h"

#include <avr/io.h>

void leds_init(void)
{
	DDRB |= (1 << 3) | (1 << 4);
	DDRD |= (1 << 0);
	leds_set(DEC);
}

void leds_set(Led led)
{
	PORTB &= ~((1 << 3) | (1 << 4));
	PORTD &= ~(1 << 0);
	switch (led) {
		case DEC:
			PORTB |= (1 << 4);
			break;
		case HEX:
			PORTB |= (1 << 3);
			break;
		case BIN:
			PORTD |= (1 << 0);
			break;
	}
}
