#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "converter_utils.h"

#define BIT_ARRAY_SIZE 64

void givenBinaryNumber_binaryToArray_returnAnArrayOfBits()
{
    byte_t expectedBitArray[BIT_ARRAY_SIZE] = { 0 };
    expectedBitArray[63]                    = 1;
    expectedBitArray[62]                    = 0;
    expectedBitArray[61]                    = 1;

    const pictureRow_t binNum                   = 0b101;
    byte_t             bitArray[BIT_ARRAY_SIZE] = {};

    binaryToArray( binNum, bitArray, BIT_ARRAY_SIZE );

    assert_memory_equal( expectedBitArray, bitArray, BIT_ARRAY_SIZE );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenBinaryNumber_binaryToArray_returnAnArrayOfBits ),
    };

    return cmocka_run_group_tests_name( "Standard mode state test ", tests, NULL, NULL );
}
