#include "display.h"

#include <util/delay.h>

int main(void)
{
    _delay_ms(50);
    display_init();

    for(;;);
}
