#include "display.h"
#include "keyboard.h"
#include "leds.h"

#include <util/delay.h>

int main(void)
{
    _delay_ms(50);

    // initialize everything
    display_init();
	keyboard_init();
	leds_init();

	// main loop
    for(;;) {
		int16_t key = keyboard_key_pressed();
		if (key != -1)
			display_set_value(key);
	}
}
