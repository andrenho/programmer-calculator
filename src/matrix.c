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
	0x0a, 0x01,	// Intensity Register, 0x00 .. 0x0f
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

static void matrix_row(uint8_t address, uint8_t data)
{
	if (address < 8)
		send_word(address + 1, data);
}

void matrix_init(void)
{
	DDRB |= (1 << MAX7219_CLK);   // Set CLK port as output
    DDRB |= (1 << MAX7219_CS);    // Set CS port as output
    DDRB |= (1 << MAX7219_DIN);   // Set DIN port as output
    
	// initialization
	for (uint8_t i = 0; i < sizeof (max7219_initseq);) {
        uint8_t opcode = pgm_read_byte(&max7219_initseq[i++]);
        uint8_t opdata = pgm_read_byte(&max7219_initseq[i++]);
        send_word(opcode, opdata);
    }

	matrix_set_value(0);
}

void matrix_set_value(int64_t value)
{
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t v = (((value >> (0 + i)) & 1) << 7)
			      | (((value >> (8 + i)) & 1) << 6)
			      | (((value >> (16 + i)) & 1) << 5)
			      | (((value >> (24 + i)) & 1) << 4)
			      | (((value >> (32 + i)) & 1) << 3)
			      | (((value >> (40 + i)) & 1) << 2)
			      | (((value >> (48 + i)) & 1) << 1)
			      | (((value >> (52 + i)) & 1) << 0);
		matrix_row(7 - i, v);
	}
}
