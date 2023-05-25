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
    for (size_t i = 0; i < 16; ++i) {
        if (line1[i] != '?')
            assert(line[0][i] == line1[i]);
        if (line2[i] != '?')
            assert(line[1][i] == line2[i]);
    }
}

static void assert_value(Key keys[], size_t n, int64_t expected_value)
{
    interface_init();
    for (size_t i = 0; i < n; ++i)
        interface_key_pressed(keys[i]);
    assert(interface_value() == expected_value);
}

#define ASSERT_CALC(expected_value, line1, line2, ...) assert_calc((Key[]) { __VA_ARGS__ }, sizeof((Key[]) { __VA_ARGS__ }) / sizeof(Key), expected_value, line1, line2);
#define ASSERT_VALUE(expected_value, ...) assert_value((Key[]) { __VA_ARGS__ }, sizeof((Key[]) { __VA_ARGS__ }) / sizeof(Key), expected_value);

int main()
{
    //
    // adding keys: value vs display
    //
    ASSERT_CALC(0,
                "               0",
                "              0h",
                K_0);
    ASSERT_CALC(1,
                "               1",
                "              1h",
                K_1);
    ASSERT_CALC(1,
                "               1",
                "              1h",
                K_1, K_A);
    ASSERT_CALC(123,
                "             123",
                "             7Bh",
                K_1, K_2, K_3);
    ASSERT_CALC(123456789012345,
                " 123456789012345",
                "   7048860DDF79h",
                K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0, K_1, K_2, K_3, K_4, K_5);
    // over the limit
    ASSERT_CALC(123456789012345,
                " 123456789012345",
                "   7048860DDF79h",
                K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0, K_1, K_2, K_3, K_4, K_5, K_6);

    //
    // backspace key
    //
    ASSERT_CALC(1,
                "               1",
                "              1h",
                K_1, K_2, K_3, K_BS, K_BS);
    ASSERT_CALC(0,
                "               0",
                "              0h",
                K_1, K_BS, K_BS);

    //
    // adding keys with different modes
    //

    // hex
    ASSERT_CALC(2748,
                "            2748",
                "            ABCh",
                K_MODE, K_A, K_B, K_C);
    // bin
    ASSERT_CALC(5,
                "               5",
                "              5h",
                K_MODE, K_MODE, K_1, K_0, K_1, K_2);

    //
    // adding keys with different sizes
    //

    // 8-bit signed
    ASSERT_VALUE(20, K_SZ, K_SZ, K_SZ, K_2, K_0, K_0);

    // 8-bit unsigned
    ASSERT_VALUE(200, K_SZ, K_SZ, K_SZ, K_FUN, K_SIGNED, K_2, K_0, K_0);
    ASSERT_VALUE(200, K_SZ, K_SZ, K_SZ, K_FUN, K_SIGNED, K_2, K_0, K_0, K_0);

    // change sign
    ASSERT_CALC(-20,
                "             -20",
                " FFFFFFFFFFFFECh",
                K_2, K_0, K_SIGN);
    ASSERT_CALC(-20,
                "             -20",
                "w          FFECh",
                K_SZ, K_SZ, K_2, K_0, K_SIGN);
    ASSERT_CALC(-20,
                "             -20",
                "b            ECh",
                K_SZ, K_SZ, K_SZ, K_2, K_0, K_SIGN);

    // change signedness
    ASSERT_CALC(20,
                "              20",
                "             14h",
                K_2, K_0, K_FUN, K_SIGN);
    ASSERT_CALC(-20,
                "             236",
                "b?           ECh",
                K_SZ, K_SZ, K_SZ, K_2, K_0, K_SIGN, K_FUN, K_SIGNED);
    ASSERT_CALC(-20,
                "           65516",
                "w?         FFECh",
                K_SZ, K_SZ, K_2, K_0, K_SIGN, K_FUN, K_SIGNED);

    // NOT
    ASSERT_CALC(-21,
                "             -21",
                " FFFFFFFFFFFFEBh",
                K_2, K_0, K_NOT);
    ASSERT_CALC(-21,
                "             -21",
                "w          FFEBh",
                K_SZ, K_SZ, K_2, K_0, K_NOT);
    ASSERT_CALC(-21,
                "           65515",
                "w?         FFEBh",
                K_SZ, K_SZ, K_2, K_0, K_NOT, K_FUN, K_SIGNED);

    // operations
    ASSERT_VALUE(5,
                K_2, K_PLUS, K_3, K_PLUS);
    ASSERT_VALUE(7,
                 K_2, K_PLUS, K_3, K_PLUS, K_2, K_EQUALS);

    // operations with different sizes
    ASSERT_CALC(-56,
                "             -56",
                "b            C8h",
                K_SZ, K_SZ, K_SZ, K_1, K_0, K_0, K_PLUS, K_1, K_0, K_0, K_EQUALS);

    // TODO - ROL/ROR with different sizes
}
