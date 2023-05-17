#include "display.h"

#include <util/delay.h>

int main(void)
{
    _delay_ms(50);

    // initialize everything
    display_init();

	// main loop
    for(;;) {
		// TODO - check for keypress
		// TODO - update value
	}
}
