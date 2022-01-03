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
| PB0 |
| PB1 |
| PB2 |
| PB3 |
| PB4 |
| PB5 |
| PB6 |
| PB7 |
| PD0 |
| PD1 |
| PD2 |
| PD3 |
| PD4 |
| PD5 |
| PD6 |
| PD7 |
