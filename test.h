#ifndef TEST_H
#define TEST_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(expr) \
    do { \
        if (expr) { \
        } \
        else { \
            TEST_FAIL(#expr, NULL); \
        } \
    } while (false)

#define TEST_ASSERT_MSG(expr, msg) \
    do { \
        if (expr) { \
        } \
        else { \
            TEST_FAIL(#expr, msg); \
        } \
    } while (false)

#define TEST_MAIN(tests...) \
    int main(int argc, char *argv[]) { \
        if (argc != 3) { \
            fprintf( \
                stderr, \
                "Unsufficient information provided, arguments should be [program] [file_index] [file_count].\n" \
            ); \
            return TEST_CODE_INTERNAL_FAIL; \
        } \
        run_tests(argv[0], argv[1], argv[2], tests, NULL); \
        return TEST_CODE_SUCCESS; \
    }

#define TEST_MAIN_EMPTY \
    int main(void) { \
        printf("\tNo tests for " __FILE__ "."); \
        return TEST_CODE_SUCCESS; \
    }

#define TEST_PASS \
    return (TestResult) { \
        .status = TEST_STATUS_PASS, \
        .function = __func__, \
    }

#define TEST_FAIL(err_expr, err_msg) \
    return (TestResult) { \
        .status = TEST_STATUS_FAIL, \
        .function = __func__, \
        .as = { \
            .err = { \
                .expr = err_expr, \
                .msg = err_msg \
            }, \
        }, \
    }

#define TEST_FUNCTION(name) \
    TestResult name(TestCtx ctx)

typedef enum TestExitCodes {
    TEST_CODE_SUCCESS,
    TEST_CODE_FAIL,
    TEST_CODE_INTERNAL_FAIL,
    TEST_CODE_USER_FAIL,
} TestExitCodes;

typedef enum TestStatus {
    TEST_STATUS_PASS,
    TEST_STATUS_FAIL,
} TestStatus;

typedef struct TestResult {
    TestStatus status;
    const char *function;
    union {
        struct {
            const char *expr;
            const char *msg;
        } err;
    } as;
} TestResult;

typedef struct TestCtx {
    size_t test_index;
    size_t test_count;
} TestCtx;

typedef TestResult (TestFunction)(TestCtx ctx);
typedef TestFunction *TestFunctionPtr;

// va is test functions
static void run_tests(const char *filename, const char *file_index, const char *file_count, ...);

// TESTING is defined when a unit is compiled in "testing" (where path to test.h is included)
#ifdef TESTING

typedef struct TestFunctionPtrList {
    TestFunctionPtr *arr;
    size_t len;
} TestFunctionPtrList;

static void run_test(size_t test_index, size_t test_count, TestFunctionPtr test) {
    TestCtx ctx = {0};
    ctx.test_index = test_index;
    ctx.test_count = test_count;
    
    TestResult result = test(ctx);

    switch (result.status) {
        case TEST_STATUS_PASS:
            printf(
                "\t[%d / %d] %s pass.\n",
                test_index + 1,
                test_count,
                result.function
            );
            break;
        case TEST_STATUS_FAIL:
            if (result.as.err.msg) {
                printf(
                    "\t[%d / %d] %s failed with message \"%s\".\n",
                    test_index + 1,
                    test_count,
                    result.function,
                    result.as.err.msg
                );
            }
            else {
                printf(
                    "\t[%d / %d] %s failed assert \"%s\".\n",
                    test_index + 1,
                    test_count,
                    result.function,
                    result.as.err.expr
                );
            }
            break;
        default:
            fprintf(stderr, "Unkown status code from test (%s): %d.\n", result.function, result.status);
            exit(TEST_CODE_INTERNAL_FAIL);
    }
}

static bool real_run_tests(
    const char *filename,
    const char *file_number,
    const char *file_count,
    TestFunctionPtrList test_list
) {
    printf("[%s / %s] %s:\n", file_number, file_count, filename);

    for (size_t i = 0; i < test_list.len; i++) {
        run_test(i, test_list.len, test_list.arr[i]);
    }
}

static TestFunctionPtrList get_test_list(va_list va_tests) {
    TestFunctionPtrList func_list = {0};

    va_list va_tests_copy;
    va_copy(va_tests_copy, va_tests);

    for (func_list.len = 0; va_arg(va_tests_copy, TestFunctionPtr) != NULL; func_list.len++)
        ;

    va_end(va_tests_copy);

    func_list.arr = malloc(func_list.len * sizeof(TestFunctionPtr));
    if (!func_list.arr) {
        fprintf(stderr, "No tests provided, if no tests use TEST_MAIN_EMPTY.\n");
        exit(TEST_CODE_USER_FAIL);
    }

    for (size_t i = 0; i < func_list.len; i++) {
        func_list.arr[i] = va_arg(va_tests, TestFunctionPtr);
    }

    return func_list;
}

static void run_tests(
    const char *filename,
    const char *file_number,
    const char *file_count,
    ...
) {
    va_list va_tests;
    va_start(va_tests, file_count);

    // Get list of func ptrs
    TestFunctionPtrList test_list = get_test_list(va_tests);

    va_end(va_tests);

    // Run tests
    if (!real_run_tests(filename, file_number, file_count, test_list)) {
        exit(TEST_CODE_FAIL);
    }
}

#endif // TESTING

#endif // TEST_H
