#include "matrix.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define MAX7219_CS      PB2 // CS
#define MAX7219_DIN     PB0 // DIN
#define MAX7219_CLK     PB1 // CLK

#define MAX7219_CS_HI()     PORTB |= (1 << MAX7219_CS)
#define MAX7219_CS_LO()     PORTB &= ~(1 << MAX7219_CS)
#define MAX7219_CLK_HI()    PORTB |= (1 << MAX7219_CLK)
#define MAX7219_CLK_LO()    PORTB &= ~(1 << MAX7219_CLK)
#define MAX7219_DIN_HI()    PORTB |= (1 << MAX7219_DIN)
#define MAX7219_DIN_LO()    PORTB &= ~(1 << MAX7219_DIN)

const uint8_t max7219_initseq[] PROGMEM = {
	0x09, 0x00,	// Decode-Mode Register, 00 = No decode
	0x0a, 0x00,	// Intensity Register, 0x00 .. 0x0f
	0x0b, 0x07,	// Scan-Limit Register, 0x07 to show all lines
	0x0c, 0x01,	// Shutdown Register, 0x01 = Normal Operation
	0x0f, 0x00,	// Display-Test Register, 0x01, 0x00 = Normal
};

static void send_byte(uint8_t data)
{
    for(uint8_t i = 8; i >= 1; i--) {
        MAX7219_CLK_LO();       // Set CLK to LOW
        if (data & 0x80)        // Mask the MSB of the data
            MAX7219_DIN_HI();   // Set DIN to HIGH
        else
            MAX7219_DIN_LO();   // Set DIN to LOW
        MAX7219_CLK_HI();       // Set CLK to HIGH
        data <<= 1;               // Shift to the left
    }
}

static void send_word(uint8_t address, uint8_t data)
{
    MAX7219_CS_LO();        // Set CS to LOW
    send_byte(address);  // Sending the address
    send_byte(data);     // Sending the data
    MAX7219_CS_HI();        // Set CS to HIGH
    MAX7219_CLK_LO();       // Set CLK to LOW
}

void matrix_init(void)
{
	DDRB |= (1 << MAX7219_CLK);   // Set CLK port as output
    DDRB |= (1 << MAX7219_CS);    // Set CS port as output
    DDRB |= (1 << MAX7219_DIN);   // Set DIN port as output
    for (uint8_t i = 0; i < sizeof (max7219_initseq);) {
        uint8_t opcode = pgm_read_byte(&max7219_initseq[i++]);
        uint8_t opdata = pgm_read_byte(&max7219_initseq[i++]);
        send_word(opcode, opdata);
    }
}
