#include "test.h"

bool failer() {
    return 1 != 1;
}

TEST_FUNCTION(test_failer_msg) {
    TEST_ASSERT_MSG(failer(), "failer failed??!!");

    TEST_PASS;
}

TEST_FUNCTION(test_failer) {
    TEST_ASSERT(failer());

    TEST_PASS;
}

TEST_FUNCTION(test_success) {
    TEST_ASSERT(1 == 1);

    TEST_PASS;
}

TEST_FUNCTION(test_success_msg) {
    TEST_ASSERT_MSG(1 == 1, "Fabric of time and space is faltering");

    TEST_PASS;
}

TEST_MAIN(test_failer, test_failer_msg, test_success, test_success_msg)
