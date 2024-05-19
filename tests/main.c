#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#define CPU_LPC55S69JBD100_cm33_core0

static void test_fajny(void** state)
{
    assert_int_equal(2, 3);
}

int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_fajny),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}