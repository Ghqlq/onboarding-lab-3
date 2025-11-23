#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include <VExercise1.h>

void test_op(uint8_t code, uint8_t(op)(uint8_t, uint8_t)) {
    VExercise1 model;
    model.op = code;
    model.a = 0;
    model.b = 0;

    do {
        do {
            model.eval();
            uint8_t expected = op(model.a, model.b);
            REQUIRE(model.out == expected);
        } while (++model.b);
    } while (++model.a);
}

TEST_CASE("op = 0, XOR") {
    test_op(0, [](uint8_t a, uint8_t b) -> uint8_t { return a ^ b; });
}

TEST_CASE("op = 1, shift left") {
    test_op(1, [](uint8_t a, uint8_t b) -> uint8_t {
        if (b >= 8) return uint8_t(0);    
        return uint8_t(a << b);
    });
}

TEST_CASE("op = 2, modulo") {
    test_op(2, [](uint8_t a, uint8_t b) -> uint8_t {
        if (b == 0) return uint8_t(0);
        return uint8_t(a % b);
    });
}

TEST_CASE("op = 3, NOT AND") {
    test_op(3, [](uint8_t a, uint8_t b) -> uint8_t { return uint8_t(~(a & b)); });
}
