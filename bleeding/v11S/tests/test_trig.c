#include "test_harness.h"
#include "ml_trig.h"
#include "ml_exp_log.h"
#include "ml_core.h"

int main() {
    ml_test_ctx_t ctx;
    ml_test_init(&ctx, "Trig & Exp/Log");
    printf("=== Trig & Exp/Log Tests ===\n");

    ASSERT_NEAR(&ctx, ml_sin(ML_PI/2.0), 1.0, 1e-14, "sin(pi/2)");
    ASSERT_NEAR(&ctx, ml_cos(ML_PI), -1.0, 1e-14, "cos(pi)");
    ASSERT_NEAR(&ctx, ml_log(1.0), 0.0, 1e-15, "log(1)");
    ASSERT_NEAR(&ctx, ml_exp(0.0), 1.0, 1e-15, "exp(0)");

    return ml_test_summary(&ctx);
}
