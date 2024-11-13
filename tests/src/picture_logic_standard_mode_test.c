#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "standard_mode_picture_getters.h"
#include "standard_mode_picture_logic.h"

size_t cyclicIncrement( const size_t, const size_t )
{
    return mock_type( size_t );
}

PictureGetterLoopStatus_t callPictureGetterAtIndex( const size_t getterIndex, const PictureDataSpan_t **const picture,
                                                    const Datetime_t *, const WeatherCondition_t )
{
    check_expected( getterIndex );

    *picture = mock_ptr_type( PictureDataSpan_t * );

    return mock_type( PictureGetterLoopStatus_t );
}


static const pictureRow_t       s_mockedSingleRow               = 128;
static const PictureDataSpan_t  s_expectedPicture               = { .size = 1, .data = &s_mockedSingleRow };
static const size_t             s_loopCount                     = 3;
static const WeatherCondition_t s_insignificantWeatherCondition = Rain;

void givenEndLoopStatus_getOccasionalPicture_returnPictureAndIncrecemntCallCounter()
{
    will_return( callPictureGetterAtIndex, &s_expectedPicture );
    will_return( callPictureGetterAtIndex, PictureGetterEndLoop );
    expect_value( callPictureGetterAtIndex, getterIndex, 0 );
    will_return( cyclicIncrement, 1 );
    assert_ptr_equal( &s_expectedPicture, getOccasionalPicture( NULL, s_insignificantWeatherCondition ) );
}

void givenLoopStatus_getOccasionalPicture_returnPicturesWithoutIncrementingCallCounter()
{
    for ( size_t i = 0; i < s_loopCount; ++i )
    {
        will_return( callPictureGetterAtIndex, &s_expectedPicture );
        will_return( callPictureGetterAtIndex, PictureGetterLoop );
        expect_value( callPictureGetterAtIndex, getterIndex, 0 );
        assert_ptr_equal( &s_expectedPicture, getOccasionalPicture( NULL, s_insignificantWeatherCondition ) );
    }
}

void givenNoAvailablePictureStatus_getOccasionalPicture_incrementCallCounterUntillStatusChanges()
{
    // 0
    will_return( callPictureGetterAtIndex, NULL );
    will_return( callPictureGetterAtIndex, NoAvailablePicture );
    expect_value( callPictureGetterAtIndex, getterIndex, 0 );

    // 1
    will_return( cyclicIncrement, 1 );
    will_return( callPictureGetterAtIndex, NULL );
    will_return( callPictureGetterAtIndex, NoAvailablePicture );
    expect_value( callPictureGetterAtIndex, getterIndex, 1 );

    // Status changes
    will_return( cyclicIncrement, 2 );
    will_return( callPictureGetterAtIndex, &s_expectedPicture );
    will_return( callPictureGetterAtIndex, PictureGetterEndLoop );
    expect_value( callPictureGetterAtIndex, getterIndex, 2 );

    will_return( cyclicIncrement, 3 );
    assert_ptr_equal( &s_expectedPicture, getOccasionalPicture( NULL, s_insignificantWeatherCondition ) );
}

// Setup to set s_callCounter back to zero
int setupGetterCallCounterToZero()
{
    will_return( callPictureGetterAtIndex, NULL );
    will_return( callPictureGetterAtIndex, PictureGetterEndLoop );
    expect_any( callPictureGetterAtIndex, getterIndex );
    will_return( cyclicIncrement, 0 );
    getOccasionalPicture( NULL, s_insignificantWeatherCondition );

    return 0;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup( givenEndLoopStatus_getOccasionalPicture_returnPictureAndIncrecemntCallCounter,
                                setupGetterCallCounterToZero ),
        cmocka_unit_test_setup( givenLoopStatus_getOccasionalPicture_returnPicturesWithoutIncrementingCallCounter,
                                setupGetterCallCounterToZero ),
        cmocka_unit_test_setup(
            givenNoAvailablePictureStatus_getOccasionalPicture_incrementCallCounterUntillStatusChanges,
            setupGetterCallCounterToZero ),
    };

    return cmocka_run_group_tests_name( "Standard mode picture logic test ", tests, NULL, NULL );
}
