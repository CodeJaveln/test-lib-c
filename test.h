#ifndef TEST_H
#define TEST_H

#include "common/common.h"

#define TEST_ASSERT(assertion) \
    do { \
        if (assertion) { \
        } \
    } while (false)

#define TEST_MAIN(tests...) \
    int main(int argc, char *argv[]) { \
        if (argc != 3) { \
            printf("\tUnsufficient information provided, arguments should be [program] [file_index] [file_count]."); \
            return EXIT_FAILURE; \
        } \
        run_tests(argv[1], argv[2], tests, NULL); \
        return EXIT_SUCCESS; \
    }

#define TEST_MAIN_EMPTY \
    void main(void) { \
        printf("\tNo tests for " __FILE__ "."); \
    }

#define TEST_PASS \
    (TestResult){ \
        .status = TEST_STATUS_PASS, \
        .function = __func__, \
    }

#define TEST_FUNCTION(name) \
    TestResult name(TestCtx ctx)

typedef enum TestStatus {
    TEST_STATUS_PASS,
    TEST_STATUS_FAIL,
} TestStatus;

typedef struct TestResult {
    TestStatus status;
    const char *function;
    union {
        struct {
            const char *assert;
            const char *msg;
        } err;
    };
} TestResult;

typedef struct TestCtx {
    size_t test_index;
    size_t test_count;
} TestCtx;

typedef TestResult (TestFunction)(TestCtx ctx);
typedef TestFunction *TestFunctionPtr;

// va is test functions
void run_tests(const char *file_index, const char *file_count, ...);

#endif // TEST_H

#ifdef TEST_IMPLEMENTATION

static TestResult run_test(size_t test_index, size_t test_count) {
}

static bool real_run_tests(
    const char *filename,
    const char *file_index,
    const char *file_count,
    TestFunctionPtrList test_list
) {
    printf("[%s / %s] %s:\n");

    for (size_t i = 0; i < test_list.len; i++) {
        run_test(i, test_list.len, test_list.arr[i]);
    }
}

void run_tests(const char *file_index, const char *file_count, ...) {
    va_list va_tests;
    va_start(va_tests, file_count);

    // Get list of func ptrs
    TestFunctionPtrList test_list = get_test_list(va_tests);

    va_end(va_tests);

    // Run tests
    if (!real_run_tests(file_index, file_count, test_list)) {
        exit(EXIT_FAILURE);
    }
}

#endif // TEST_IMPLEMENTATION
