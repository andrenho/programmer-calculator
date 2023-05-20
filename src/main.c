#include "display.h"
#include "keyboard.h"
#include "interface.h"
#include "leds.h"
#include "matrix.h"

#include <util/delay.h>

static void update_displays(void)
{
	char lines[2][16];
	interface_display(lines);
	display_set_lines(lines);
	matrix_set_value(interface_value(), interface_size());
	leds_set(interface_mode());
}

int main(void)
{
    _delay_ms(50);

    // initialize everything
    display_init();
	keyboard_init();
	leds_init();
	interface_init();
	matrix_init();

	update_displays();

	// main loop
    for(;;) {
		int16_t key = keyboard_key_pressed();
		if (key != -1) {
			interface_key_pressed(key);
			update_displays();

			// debounce
			_delay_ms(20);
			while (keyboard_key_pressed() != -1);
			_delay_ms(20);
		}
	}
}
