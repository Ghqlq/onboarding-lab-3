#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include <VExercise3.h>

static void tick(VExercise3& model) {
    model.clk = 1;
    model.eval();
    model.clk = 0;
    model.eval();
}
static uint8_t mystery1(uint8_t a, uint8_t b, uint8_t c) {
    switch (a & 0x3u) {
    case 0:
        return static_cast<uint8_t>(
            (0u << 6) |
            ((b & 0x7u) << 3) |
            (c & 0x7u)
        );
    case 1:
        return static_cast<uint8_t>(
            (1u << 6) |
            ((c & 0x7u) << 3) |
            (b & 0x7u)
        );
    case 2:
        return b;
    case 3:
    default:
        return c;
    }
}

static uint8_t parity16(uint16_t x) {
    uint8_t p = 0;
    for (int i = 0; i < 16; ++i) {
        p ^= (x >> i) & 1u;
    }
    return p & 1u;
}

static void mystery2_step(uint16_t& out, int& state, uint8_t a_in, uint8_t b_in) {
    switch (state) {
    case 0:
        out = static_cast<uint16_t>((uint16_t(a_in) << 8) | (out & 0x00FFu));
        break;
    case 1:
        out = static_cast<uint16_t>((out & 0xFF00u) | b_in);
        break;
    case 2:
        out = static_cast<uint16_t>(((out & 0x00FFu) << 8) | ((out & 0xFF00u) >> 8));
        break;
    case 3: {
        uint16_t n0 = (out & 0x000Fu) << 12;
        uint16_t n1 = (out & 0x00F0u) << 4;
        uint16_t n2 = (out & 0x0F00u) >> 4;
        uint16_t n3 = (out & 0xF000u) >> 12;
        out = static_cast<uint16_t>(n0 | n1 | n2 | n3);
        break;
    }
    case 4: {
        uint16_t p = parity16(out);
        out = static_cast<uint16_t>(p & 0x1u);
        break;
    }
    default:
        break;
    }
    state = (state + 1) % 5;
}
struct Vec {
    uint8_t  a;
    uint16_t b;
    uint16_t c;
};

TEST_CASE("Exercise3: reset loads ") {
    VExercise3 model;
    model.clk = 0;

    Vec tests[] = {
        {0x0u, 0x0000u, 0x0000u},
        {0x5u, 0x1234u, 0xABCDu},
        {0xAu, 0xFFFFu, 0x0000u},
        {0xFu, 0x00FFu, 0xFF00u},
    };

    for (auto const& t : tests) {
        model.reset = 1;
        model.a     = t.a;
        model.b     = t.b;
        model.c     = t.c;
        model.eval();

        uint8_t a_low2  = t.a & 0x3u;
        uint8_t a_high2 = (t.a >> 2) & 0x3u;
        uint8_t b_lo    = t.b & 0x00FFu;
        uint8_t b_hi    = (t.b >> 8) & 0x00FFu;
        uint8_t c_lo    = t.c & 0x00FFu;
        uint8_t c_hi    = (t.c >> 8) & 0x00FFu;

        uint8_t a_in = mystery1(a_low2,  b_lo, c_lo);
        uint8_t b_in = mystery1(a_high2, b_hi, c_hi);

        tick(model);

        uint16_t expected = static_cast<uint16_t>((uint16_t(b_in) << 8) | a_in);
        REQUIRE(static_cast<uint16_t>(model.out) == expected);
    }
}

TEST_CASE("Exercise3") {
    VExercise3 model;
    model.clk = 0;
    Vec tests[] = {
        {0x1u, 0x1357u, 0x9BDFu},
        {0x3u, 0xAAAAu, 0x5555u},
        {0xCu, 0x0F0Fu, 0xF0F0u},
    };
    for (auto const& t : tests) {
        model.a     = t.a;
        model.b     = t.b;
        model.c     = t.c;
        model.reset = 1;
        model.eval();

        uint8_t a_low2  = t.a & 0x3u;
        uint8_t a_high2 = (t.a >> 2) & 0x3u;
        uint8_t b_lo    = t.b & 0x00FFu;
        uint8_t b_hi    = (t.b >> 8) & 0x00FFu;
        uint8_t c_lo    = t.c & 0x00FFu;
        uint8_t c_hi    = (t.c >> 8) & 0x00FFu;

        uint8_t a_in = mystery1(a_low2,  b_lo, c_lo);
        uint8_t b_in = mystery1(a_high2, b_hi, c_hi);

        tick(model);
        uint16_t expected = static_cast<uint16_t>((uint16_t(b_in) << 8) | a_in);
        REQUIRE(static_cast<uint16_t>(model.out) == expected);
        model.reset = 0;
        model.eval();
        int state = 0;
        for (int i = 0; i < 20; ++i) {
            tick(model);
            mystery2_step(expected, state, a_in, b_in);
            REQUIRE(static_cast<uint16_t>(model.out) == expected);
        }
    }
}
