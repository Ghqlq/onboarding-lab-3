#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include <VExercise2.h>

static void tick(VExercise2& model) {
    model.clk = 1;
    model.eval();
    model.clk = 0;
    model.eval();
}
static uint16_t lfsr_step(uint16_t current) {
    uint16_t bit10 = (current >> 10) & 1u;
    uint16_t bit8  = (current >> 8)  & 1u;
    uint16_t bit3  = (current >> 3)  & 1u;
    uint16_t bit1  = (current >> 1)  & 1u;
    uint16_t feedback = bit10 ^ bit8 ^ bit3 ^ bit1;

    uint16_t shifted = (current << 1) & 0xFFFEu;  // keep bits 15..1, clear bit 0
    return shifted | feedback;
}

TEST_CASE("Exercise2: reset loads bitwise inverse of init") {
    VExercise2 model;
    model.clk = 0;
    uint16_t inits[] = {0x0000u, 0x0001u, 0x1234u, 0xACE1u, 0xFFFFu};

    for (uint16_t init : inits) {
        model.reset = 1;
        model.init  = init;
        model.eval();
        tick(model);

        uint16_t expected = static_cast<uint16_t>(~init);
        REQUIRE(static_cast<uint16_t>(model.out) == expected);
    }
}
TEST_CASE("Exercise2: LFSR sequence matches software model") {
    VExercise2 model;
    model.clk = 0;

    uint16_t seeds[] = {0x0001u, 0xACE1u, 0xBEEFu};

    for (uint16_t init : seeds) {
        model.reset = 1;
        model.init  = init;
        model.eval();
        tick(model);  

        uint16_t expected = static_cast<uint16_t>(~init);
        REQUIRE(static_cast<uint16_t>(model.out) == expected);

        model.reset = 0;
        model.eval();
      
        for (int i = 0; i < 100; ++i) {  
            tick(model);                  
            expected = lfsr_step(expected);
            REQUIRE(static_cast<uint16_t>(model.out) == expected);
        }
    }
}

