#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test_cases.h"


int test_case = 0;
int fail_count = 0;
int exit_on_fail = 1;

int
get_fail_count()
{
    return fail_count;
}

int
get_test_count()
{
    return test_case;
}

void
assertStrEq(char *expected_str, char *test_str)
{
    test_case++;
    if (strcmp(expected_str, test_str) != 0) {
        printf("FAIL: test %d expected %s but got %s\n", test_case, expected_str, test_str);
        fail_count++;
        if (exit_on_fail)
            exit(-1);
    } else {
        printf("test %.3d pass\n", test_case);
    }
}

void
assertIntEq(int expected_int, int test_int)
{
    test_case++;
    if (expected_int != test_int) {
        printf("FAIL: test %d expected value %d but got %d\n", test_case, expected_int, test_int);
        fail_count++;
        if (exit_on_fail)
            exit(-1);
    } else {
        printf("test %.3d pass\n", test_case);
    }
}

void
assertNotNull(void *data)
{
    test_case++;
    if (data == NULL) {
        printf("FAIL: test %d expected non null value\n", test_case);
        fail_count++;
        if (exit_on_fail)
            exit(-1);
    } else {
        printf("test %.3d pass\n", test_case);
    }
}

void
assertNull(void *data)
{
    test_case++;
    if (data != NULL) {
        printf("FAIL: test %d expected null value but got %s\n", test_case, (char *)data);
        fail_count++;
        if (exit_on_fail)
            exit(-1);
    } else {
        printf("test %.3d pass\n", test_case);
    }
}

void
assertFail(char *msg)
{
    test_case++;
    printf("test %.3d: FAIL %s\n", test_case, msg);
    fail_count++;
}

