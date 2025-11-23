#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include <VExercise4.h>

static void eval_comb(VExercise4& model) {
    model.eval();   // pure combinational, no clock
}

static const uint8_t PATTERNS[] = {
    0x00u,  
    0xFFu,  // 1s
    0x55u,  // alternating bits
    0xAAu,  // alternating bits
    0x0Fu, 
    0xF0u   
};
TEST_CASE("Exercise4: cs = 0 forces out = 0 regardless of sel and inputs") {
    VExercise4 model;

    for (uint8_t sel = 0; sel < 4; ++sel) {
        for (uint8_t alpha : PATTERNS) {
            for (uint8_t beta : PATTERNS) {
                for (uint8_t gamma : PATTERNS) {
                    model.cs    = 0;
                    model.sel   = sel;
                    model.alpha = alpha;
                    model.beta  = beta;
                    model.gamma = gamma;

                    eval_comb(model);
                    REQUIRE(static_cast<uint8_t>(model.out) == 0u);
                }
            }
        }
    }
}

TEST_CASE("Exercise4: cs = 1, sel = 0, alpha") {
    VExercise4 model;
    model.cs  = 1;
    model.sel = 0;

    for (uint8_t alpha : PATTERNS) {
        for (uint8_t beta : PATTERNS) {
            for (uint8_t gamma : PATTERNS) {
                model.alpha = alpha;
                model.beta  = beta;
                model.gamma = gamma;

                eval_comb(model);
                REQUIRE(static_cast<uint8_t>(model.out) == alpha);
            }
        }
    }
}

TEST_CASE("Exercise4: cs = 1, sel = 1, beta") {
    VExercise4 model;
    model.cs  = 1;
    model.sel = 1;

    for (uint8_t alpha : PATTERNS) {
        for (uint8_t beta : PATTERNS) {
            for (uint8_t gamma : PATTERNS) {
                model.alpha = alpha;
                model.beta  = beta;
                model.gamma = gamma;

                eval_comb(model);
                REQUIRE(static_cast<uint8_t>(model.out) == beta);
            }
        }
    }
}

TEST_CASE("Exercise4: cs = 1, sel = 2, gamma") {
    VExercise4 model;
    model.cs  = 1;
    model.sel = 2;

    for (uint8_t alpha : PATTERNS) {
        for (uint8_t beta : PATTERNS) {
            for (uint8_t gamma : PATTERNS) {
                model.alpha = alpha;
                model.beta  = beta;
                model.gamma = gamma;

                eval_comb(model);
                REQUIRE(static_cast<uint8_t>(model.out) == gamma);
            }
        }
    }
}

TEST_CASE("Exercise4: cs = 1, sel = 3, alpha & (beta | gamma)") {
    VExercise4 model;
    model.cs  = 1;
    model.sel = 3;

    for (uint8_t alpha : PATTERNS) {
        for (uint8_t beta : PATTERNS) {
            for (uint8_t gamma : PATTERNS) {
                model.alpha = alpha;
                model.beta  = beta;
                model.gamma = gamma;

                eval_comb(model);
                uint8_t expected = static_cast<uint8_t>(alpha & (beta | gamma));
                REQUIRE(static_cast<uint8_t>(model.out) == expected);
            }
        }
    }
}
