```

Battery -- Buck converter -- On/off logic circuit (with hold)

Binary display -----.
                    |
Main display   --  MCU  -- Base chooser
                    |
                    |-- Keyboard matrix 1
                    '-- Keyboard matrix 2
```

Functions:
- Arithmetic (`+ - * / mod`)
- Logic (`and or xor not rol ror`)
- Equals
- Choose base (hex/dec/bin)
- Off

Matrix:
- matrix 1: numbers (`0..9` and `A..F`)
- matrix 2: functions (`+ - * / mod and or xor not rol ror = base MS MR MC`)
- Off button

ATMEGA16 connections:
| Pin | Description |
|-----|-----|
| PA0..7 | Keyboard matrix 1 |
| PC0..7 | Keyboard matrix 2 |
| PB0 | BIN0 |
| PB1 | BIN1 |
| PB2 | BIN2 |
| PB3 | HOLD BTN |
| PB4 | OFF BTN |
| PB5 | LED DEC / MOSI |
| PB6 | LED HEX / MISO |
| PB7 | LED BIN / SCK  |
| PD0 | RX |
| PD1 | TX |
| PD2 | LCD E  |
| PD3 | LCD RS |
| PD4 | LCD D4 |
| PD5 | LCD D5 |
| PD6 | LCD D6 |
| PD7 | LCD D7 |
