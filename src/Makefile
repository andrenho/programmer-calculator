# Project name
PROJECT=progcalc

# Object files (one for each source file)
OBJECTS=main.o display.o keyboard.o leds.o matrix.o interface.o

# Avrdude configuration to connect on the PI. See avrdude_gpio.conf.
# Can be one of: pi_rst, pi_cs0, pi_cs1 or pi_gpio, depending on the RESET line in AVR.
AVRDUDE_CONFIG=pi_rst

# AVR fuses: internal clock (see https://www.engbedded.com/fusecalc/)
FUSES=-U lfuse:w:0xe4:m -U hfuse:w:0xd9:m

# Microcontroller
MCU_GCC=atmega16
MCU_AVRDUDE=atmega16

# Microcontroller speed, in Hz. Used by avr-libc for delays.
F_CPU=8000000UL

# Serial speed, in baud. Used by avr-libc for UART speed calculation.
BAUD=38400

#
# Rules
#

AVRDUDE_FLAGS=-p ${MCU_AVRDUDE} -C +./avrdude_gpio.conf -c ${AVRDUDE_CONFIG} -V
CC=avr-gcc
WARNINGS=-Wall -Wextra \
	 -Wformat=2 -Wno-unused-parameter -Wshadow \
	 -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	 -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wjump-misses-init -Wlogical-op
CPPFLAGS=-std=c11 ${WARNINGS} -Os -DF_CPU=${F_CPU} -DBAUD=${BAUD} -mmcu=${MCU_GCC}\
	 -ffunction-sections -fdata-sections -mcall-prologues -D_LCD_DEFINITIONS_FILE

all: ${PROJECT}.hex

${PROJECT}.hex: ${PROJECT}.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
	arm-linux-gnueabihf-size ${PROJECT}.hex

${PROJECT}.elf: ${OBJECTS}
	$(CC) -mmcu=${MCU_GCC} -o $@ $^ -Wl,--gc-sections
	avr-size -C --mcu=${MCU_GCC} ${PROJECT}.elf

progcalc_test: tests.c interface.c interface.h
	gcc -o -g -ggdb -O0 -Wall -Wextra tests.c interface.c -o $@

test: progcalc_test
	./$@

test-connection:
	sudo avrdude ${AVRDUDE_FLAGS}

upload: ${PROJECT}.hex
	sudo avrdude ${AVRDUDE_FLAGS} -U flash:w:$<:i

fuse:
	sudo avrdude ${AVRDUDE_FLAGS} ${FUSES}

size: ${PROJECT}.elf
	avr-size -C --mcu=${MCU_GCC} ${PROJECT}.elf	

talk:
	@echo "Press Ctrl+A and then Shift+K to exit."
	@sleep 1
	sudo screen /dev/ttyAMA0 38400

clean:
	rm -f ${OBJECTS} ${PROJECT}.elf ${PROJECT}.hex

# vim: set ts=8 sts=8 sw=8 noexpandtab:
