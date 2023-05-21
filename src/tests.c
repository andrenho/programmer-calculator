#include "interface.h"

#include <assert.h>
#include <string.h>

static void assert_calc(Key keys[], size_t n_keys, int64_t expected_value, char line1[], char line2[])
{
    interface_init();
    for (size_t i = 0; i < n_keys; ++i)
        interface_key_pressed(keys[i]);
    assert(interface_value() == expected_value);

    char line[2][16];
    interface_display(line);
    assert(strncmp(line[0], line1, 16) == 0);
    assert(strncmp(line[1], line2, 16) == 0);
}
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define ASSERT_CALC(keys, expected_value, line1, line2) {assert_calc((Key[]) keys, COUNT_OF((Key[]) keys), expected_value, line1, line2);}

int main()
{
    // TODO - adding keys: value vs display
    ASSERT_CALC({ K_0 }, 0,
                "               0",
                "              0h");

    // TODO - backspace key

    // TODO - adding keys with different modes

    // TODO - adding keys with different sizes

    // TODO - change sign

    // TODO - NOT

    // TODO - operations

    // TODO - operations with different sizes

    // TODO - ROL/ROR with different sizes
}
