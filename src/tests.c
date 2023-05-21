#include "interface.h"

#include <assert.h>
#include <string.h>

static void assert_calc(Key keys[], size_t n, int64_t expected_value, char line1[], char line2[])
{
    interface_init();
    for (size_t i = 0; i < n; ++i)
        interface_key_pressed(keys[i]);
    assert(interface_value() == expected_value);

    char line[2][16];
    interface_display(line);
    assert(strncmp(line[0], line1, 16) == 0);
    assert(strncmp(line[1], line2, 16) == 0);
}

#define ASSERT_CALC(a, b, c, ...) assert_calc((Key[]) { __VA_ARGS__ }, sizeof((Key[]) { __VA_ARGS__ }) / sizeof(Key), a, b, c)

int main()
{
    // TODO - adding keys: value vs display
    ASSERT_CALC(0, "               0",
                   "              0h", K_0);
    ASSERT_CALC(0, "               1",
                   "              1h", K_1);

    // TODO - backspace key

    // TODO - adding keys with different modes

    // TODO - adding keys with different sizes

    // TODO - change sign

    // TODO - NOT

    // TODO - operations

    // TODO - operations with different sizes

    // TODO - ROL/ROR with different sizes
}
