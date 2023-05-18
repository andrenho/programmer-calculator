#ifndef LEDS_H_
#define LEDS_H_

typedef enum { DEC, HEX, BIN } Led;

void leds_init(void);
void leds_set(Led led);

#endif
