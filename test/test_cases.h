
int get_fail_count(void);
int get_test_count(void);
void assertStrEq(char *expected_str, char *test_str);
void assertIntEq(int expected_int, int test_int);
void assertNotNull(void *data);
void assertNull(void *data);
void assertFail(char *msg);

