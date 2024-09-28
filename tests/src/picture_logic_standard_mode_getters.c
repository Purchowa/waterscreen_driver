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

    const PictureDataView_t *pictureDataViewPtr;
    const Datetime_t         datetime             = { .time = { .hour = 12, .minute = 4 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getCurrentTimeAsPicture( &pictureDataViewPtr, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( pictureDataViewPtr->size, EXPECTED_TIME_PICTURE_SIZE );
    assert_memory_equal( pictureDataViewPtr->data, expectedTimePicture,
                         sizeof( pictureRow_t ) * EXPECTED_TIME_PICTURE_SIZE );
}

void givenLowerRangeWinterDate_getSeasonalPicture_returnWinterMockedSize()
{
    const PictureDataView_t *pictureDataViewPtr   = NULL;
    const Datetime_t         datetime             = { .date = { .month = January, .day = 10 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &pictureDataViewPtr, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( pictureDataViewPtr->size, MOCKED_WINTER_SIZE );
    assert_non_null( pictureDataViewPtr );
}

void givenHigherRangeWinterDate_getSeasonalPicture_returnWinterMockedSize()
{
    const PictureDataView_t *pictureDataViewPtr   = NULL;
    const Datetime_t         datetime             = { .date = { .month = December, .day = 29 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &pictureDataViewPtr, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( pictureDataViewPtr->size, MOCKED_WINTER_SIZE );
    assert_non_null( pictureDataViewPtr );
}

void givenDateEqualToSummer_getSeasonalPicture_returnSummerMockedSize()
{
    const PictureDataView_t *pictureDataViewPtr   = NULL;
    const Datetime_t         datetime             = { .date = { .month = June, .day = 21 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &pictureDataViewPtr, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( pictureDataViewPtr->size, MOCKED_SUMMER_SIZE );
    assert_non_null( pictureDataViewPtr );
}

void givenDateInRangeOfAutumn_getSeasonalPicture_returnAutumnMockedSize()
{
    const PictureDataView_t *pictureDataViewPtr = NULL;
    const Datetime_t         datetime             = { .date = { .month = October, .day = 10 } };
    const WeatherCondition_t insignificantWeather = Rain;

    const PictureGetterLoopStatus_t loopStatus =
        getSeasonalPicture( &pictureDataViewPtr, &datetime, insignificantWeather );

    assert_int_equal( loopStatus, PictureGetterEndLoop );
    assert_int_equal( pictureDataViewPtr->size, MOCKED_AUTUMN_SIZE );
    assert_non_null( pictureDataViewPtr );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenTime_getCurrentTimeAsPicture_returnEndLoopAndProvideTimePicture ),
        cmocka_unit_test( givenLowerRangeWinterDate_getSeasonalPicture_returnWinterMockedSize ),
        cmocka_unit_test( givenHigherRangeWinterDate_getSeasonalPicture_returnWinterMockedSize ),
        cmocka_unit_test( givenDateEqualToSummer_getSeasonalPicture_returnSummerMockedSize ),
        cmocka_unit_test( givenDateInRangeOfAutumn_getSeasonalPicture_returnAutumnMockedSize ),
    };

    return cmocka_run_group_tests_name( "Standard mode picture getters logic test ", tests, NULL, NULL );
}
