# Test Library for C
The name rolls off the tongue.

Quite simple actually.
Every c file has a
```c
// In file foo.c
#ifdef TESTING

#endif // TESTING
```

Define some testing functions then a `TEST_MAIN()` that defines a main for that file
Will be complicated if testing one thing depends on something else..

## EXAMPLE
```c
// In foo.c
bool secretly_bad(int param) {
    (void)param;
    return false;
}

// Still in foo.c
#ifdef TESTING

TEST_FUNC(test_secretly_bad) {
    TEST_ASSERT_MSG(secretly_bad(0x100) == true, "secretly_bad false");

    TEST_PASS;
}

TEST_FUNC(test_true) {
    TEST_ASSERT_MSG(true, "true should be true");

    TEST_PASS;
}

TEST_MAIN(test_true, test_secretly_bad)

// Output (fail red, pass green):
//  [1 / 1] src/foo.c:
//      [1 / 3] test_true pass.
//      [2 / 3] test_secretly_bad failed assert "secretly_bad(0x100) == true".
//      [3 / 3] test_real_bad failed with message "secretly_bad fucked me up man".

#endif // TESTING
```
