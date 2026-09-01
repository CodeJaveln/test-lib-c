#include "test.h"

bool successful_func() {
    // Very important to test the fabric of the universe.
    return 0 != 2;
}

TEST_FUNCTION(test_successful_func) {
    TEST_ASSERT(successful_func() == true);

    TEST_PASS;
}

TEST_MAIN(test_successful_func)
