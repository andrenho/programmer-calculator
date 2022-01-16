#include "display.h"

#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

#define set_E()    PORTD |= _BV(PD2)
#define clear_E()  PORTD &= ~_BV(PD2)
#define set_RS()   PORTD |= _BV(PD3)
#define clear_RS() PORTD &= ~_BV(PD3)
#define set_DATA(value) PORTD = (PORTD & 0xf) | ((value) << 4)

static void send_command(bool rs, uint8_t data)
{
    if (rs) set_RS(); else clear_RS();
    set_DATA(data);
    set_E();
    _delay_ms(2);
    clear_E();
}

void display_init(void)
{
    DDRD |= 0b11111100;
    clear_RS();
    clear_E();
    set_DATA(0);

    // initialize LCD
    send_command(0, 0b0010);   // set to 4-bit operation
    send_command(0, 0b0010);   // function set: 4 bits, 2 lines, 5x11 font
    send_command(0, 0b1000);
    send_command(0, 0b0000);   // display/cursor on
    send_command(0, 0b1110); 

    send_command(0, 0b0000);   // entry mode
    send_command(0, 0b0110); 

    send_command(0, 0b0000);   // clear screen
    send_command(0, 0b0001); 

    send_command(1, 0b0100);   // print 'H'
    send_command(1, 0b1000); 

    send_command(1, 0b0110);   // print 'a'
    send_command(1, 0b0001); 
}
