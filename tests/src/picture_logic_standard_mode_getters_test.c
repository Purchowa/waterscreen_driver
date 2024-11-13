#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "standard_mode_picture_getters.h"
#include "mocks/picture_data_mocks.h"


#define EXPECTED_TIME_PICTURE_SIZE 24
void givenTime_getCurrentTimeAsPicture_returnEndLoopAndProvideTimePicture()
{
    const pictureRow_t expectedTimePicture[EXPECTED_TIME_PICTURE_SIZE] = {
        0b0000000000000000000000000000000000000000000000000000000000000000,
        0b0000011111000000011111111110000000000011111111000000000000111110,
        0b0000111111000000111111111111000000000111111111100000000001111110,
        0b0111111111000001111111111111100000001111111111110000000011111110,
        0b0111111111000001111111111111100000011111111111111000000111111110,
        0b0111111111000001111111111111100000011111111111111000001111111110,
        0b0111111111000001111110011111101111011111000011111000011111111110,
        0b0111111111000001111100001111101111011111000011111000111111111110,
        0b0000011111000001111100001111101111011111000011111001111110111110,
        0b0000011111000000000000001111100000011111000011111011111100111110,
        0b0000011111000000000000011111100000011111000011111011111000111110,
        0b0000011111000000000000111111100000011111000011111011110000111110,
        0b0000011111000000000001111111000000011111000011111011110000111110,
        0b0000011111000000000011111110000000011111000011111011111111111111,
        0b0000011111000000000111111100000000011111000011111011111111111111,
        0b0000011111000000001111111000001111011111000011111011111111111111,
        0b0000011111000000011111110000001111011111000011111011111111111111,
        0b0000011111000000111111100000001111011111000011111000000000111110,
        0b1111111111111101111111111111100000011111111111111000000000111110,
        0b1111111111111101111111111111100000011111111111111000000000111110,
        0b1111111111111101111111111111100000001111111111110000000000111110,
        0b1111111111111101111111111111100000000111111111100000000000111110,
        0b1111111111111101111111111111100000000011111111000000000000111110,
        0b0000000000000000000000000000000000000000000000000000000000000000 };

    const PictureDataSpan_t *picture;
    const Datetime_t         datetime             = { .time = { .hour = 12, .minute = 4 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getCurrentTimeAsPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, EXPECTED_TIME_PICTURE_SIZE );
    assert_memory_equal( picture->data, expectedTimePicture,
                         sizeof( pictureRow_t ) * EXPECTED_TIME_PICTURE_SIZE );
}

void givenLowerRangeWinterDate_getSeasonalPicture_returnWinterMockedSize()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = January, .day = 10 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, MOCKED_WINTER_SIZE );
    assert_non_null( picture->data );
}

void givenHigherRangeWinterDate_getSeasonalPicture_returnWinterMockedSize()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = December, .day = 29 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, MOCKED_WINTER_SIZE );
    assert_non_null( picture->data );
}

void givenDateEqualToSummer_getSeasonalPicture_returnSummerMockedSize()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = June, .day = 21 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, MOCKED_SUMMER_SIZE );
    assert_non_null( picture->data );
}

void givenDateInRangeOfAutumn_getSeasonalPicture_returnAutumnMockedSize()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = October, .day = 10 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, MOCKED_AUTUMN_SIZE );
    assert_non_null( picture->data );
}

void givenSingleDateRange_getHolidaysPicture_returnMockedSizeAndEndLoop()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = January, .day = 1 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getHolidaysPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, SINGLE_DAY_RANGE_MOCKED_SIZE );
    assert_non_null( picture->data );
}

void givenNormalDateRange_getHolidaysPicture_returnMockedSizeAndEndLoop()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = February, .day = 25 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getHolidaysPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( picture->size, NORMAL_RANGE_MOCKED_SIZE );
    assert_non_null( picture->data );
}

void givenDateOutsideAnyRange_getHolidaysPicture_returnProperCodeWithoutPicture()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = April, .day = 1 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getHolidaysPicture( &picture, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, NoAvailablePicture );
    assert_null( picture );
}

void givenDateInRangeWithTwoPictures_getHolidaysPicture_returnMultiplePictures()
{
    const PictureDataSpan_t *picture   = NULL;
    const Datetime_t         datetime             = { .date = { .month = March, .day = 17 } };
    const WeatherCondition_t insignificantWeather = Rain;

    PictureGetterLoopStatus_t loopStatus = getHolidaysPicture( &picture, &datetime, insignificantWeather );
    assert_int_equal( loopStatus, PictureGetterLoop );
    assert_non_null( picture->data );
    assert_int_equal( picture->size, MULTIPLE_PICTURES_MOCKED_SIZE_0 );

    loopStatus = getHolidaysPicture( &picture, &datetime, insignificantWeather );
    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_non_null( picture->data );
    assert_int_equal( picture->size, MULTIPLE_PICTURES_MOCKED_SIZE_1 );
}


int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenTime_getCurrentTimeAsPicture_returnEndLoopAndProvideTimePicture ),

        cmocka_unit_test( givenLowerRangeWinterDate_getSeasonalPicture_returnWinterMockedSize ),
        cmocka_unit_test( givenHigherRangeWinterDate_getSeasonalPicture_returnWinterMockedSize ),
        cmocka_unit_test( givenDateEqualToSummer_getSeasonalPicture_returnSummerMockedSize ),
        cmocka_unit_test( givenDateInRangeOfAutumn_getSeasonalPicture_returnAutumnMockedSize ),

        cmocka_unit_test( givenSingleDateRange_getHolidaysPicture_returnMockedSizeAndEndLoop ),
        cmocka_unit_test( givenNormalDateRange_getHolidaysPicture_returnMockedSizeAndEndLoop ),
        cmocka_unit_test( givenDateOutsideAnyRange_getHolidaysPicture_returnProperCodeWithoutPicture ),
        cmocka_unit_test( givenDateInRangeWithTwoPictures_getHolidaysPicture_returnMultiplePictures ),
    };

    return cmocka_run_group_tests_name( "Standard mode picture getters logic test ", tests, NULL, NULL );
}
