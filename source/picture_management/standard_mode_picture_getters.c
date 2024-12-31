#include "picture_management/standard_mode_picture_getters.h"

#include "datetime/datetime_utils.h"
#include "picture_data.h"

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


#define ONES_EXTRACTOR 10
#define TENS_EXTRACTOR 100

#define HOUR_TENS_SHIFT 49
#define HOUR_ONES_SHIFT 34

#define COLON_SHIFT 24

#define MINUTE_TENS_SHIFT 14
#define MINUTE_ONES_SHIFT 1

#define NOT_FOUND -1


const pictureGetterFun_t g_pictureGetterFunctions[STANDARD_MODE_PICTURE_GETTER_COUNT] = {
    getWeatherAsPicture,    getCurrentTimeAsPicture, getSeasonalPicture,
    getStandardModePicture, getHolidaysPicture,      getCurrentTimeAsPicture };

static uint8_t extractOnesFromNumber( const uint8_t number )
{
    return number % ONES_EXTRACTOR;
}

static uint8_t extractTensFromNumber( const uint8_t number )
{
    return ( number / ONES_EXTRACTOR ) % TENS_EXTRACTOR;
}

static PictureInfo_t s_timePictureInfo = { .picture = { .size = NUMBER_OF_ROWS_IN_PICTURE_CHARACTER, .data = NULL },
                                           .colors  = { .main = { 0, 153, 255 }, .secondary = { 0, 255, 47 } } };

PictureGetterLoopStatus_t getCurrentTimeAsPicture( PictureInfo_t **const picture, const Datetime_t *datetime,
                                                   const WeatherCondition_t )
{
    memset( g_timePicture, 0, sizeof( *g_timePicture ) * NUMBER_OF_ROWS_IN_PICTURE_CHARACTER );

    const uint8_t hourOnes = extractOnesFromNumber( datetime->time.hour );
    const uint8_t hourTens = extractTensFromNumber( datetime->time.hour );

    const uint8_t minuteOnes = extractOnesFromNumber( datetime->time.minute );
    const uint8_t minuteTens = extractTensFromNumber( datetime->time.minute );

    assert( hourOnes < CHARACTER_TO_PICTURE_MAP_SIZE && hourTens < CHARACTER_TO_PICTURE_MAP_SIZE &&
            minuteOnes < CHARACTER_TO_PICTURE_MAP_SIZE && minuteTens < CHARACTER_TO_PICTURE_MAP_SIZE );

    for ( size_t i = 0; i < NUMBER_OF_ROWS_IN_PICTURE_CHARACTER; ++i )
    {
        g_timePicture[i] = (pictureRow_t)g_characterToPictureMap[hourTens][i] << HOUR_TENS_SHIFT |
            (pictureRow_t)g_characterToPictureMap[hourOnes][i] << HOUR_ONES_SHIFT |
            (pictureRow_t)g_characterToPictureMap[ColonIdx][i] << COLON_SHIFT |
            (pictureRow_t)g_characterToPictureMap[minuteTens][i] << MINUTE_TENS_SHIFT |
            (pictureRow_t)g_characterToPictureMap[minuteOnes][i] >> MINUTE_ONES_SHIFT;
    }

    s_timePictureInfo.picture.data = g_timePicture;

    *picture = &s_timePictureInfo;

    return PictureGetterEndLoop;
}

PictureGetterLoopStatus_t getWeatherAsPicture( PictureInfo_t **const    picture, const Datetime_t *,
                                               const WeatherCondition_t weatherCondition )
{
    assert( 0 <= weatherCondition && weatherCondition < WEATHER_CONDITION_SIZE );

    *picture = g_weatherPictures[weatherCondition];

    return PictureGetterEndLoop;
}

PictureGetterLoopStatus_t getSeasonalPicture( PictureInfo_t **const picture, const Datetime_t *datetime,
                                              const WeatherCondition_t )
{
    const ShortDate_t currentShortDate = { .day = datetime->date.day, .month = datetime->date.month };

    int32_t i = 0;
    for ( ; i < SEASONS_COUNT; ++i )
    {
        const Comparison_t compareResult = compareShortDates( currentShortDate, g_seasonsInfo[i].seasonDateStart );
        if ( compareResult == SecondIsBigger )
            break;
    }

    if ( i - 1 < 0 )
        i = SEASONS_COUNT;

    *picture = g_seasonsInfo[i - 1].picture;

    return PictureGetterEndLoop;
}

/**
 * Function will only compare correctly for a date range that falls within the same year.
 * @param date - {day, month}
 * @param range - holiday closed range [from, to]
 * @return true if date is in 'range', false otherwise
 */
static bool isDateInClosedRange( const ShortDate_t date, const HolidaysRange_t *range )
{
    const Comparison_t fromComparison = compareShortDates( date, range->from );
    const Comparison_t toComparison   = compareShortDates( date, range->to );

    return ( ( fromComparison == FirstIsBigger || fromComparison == Equal ) &&
             ( toComparison == SecondIsBigger || toComparison == Equal ) );
}

static int32_t tryToFindMatchingHoliday( const Datetime_t *datetime )
{
    const ShortDate_t currentShortDate = { .day = datetime->date.day, .month = datetime->date.month };

    for ( size_t i = 0; i < HOLIDAYS_COUNT; ++i )
    {
        if ( isDateInClosedRange( currentShortDate, &g_holidaysInfo[i].range ) )
        {
            return i;
        }
    }

    return NOT_FOUND;
}

PictureGetterLoopStatus_t getHolidaysPicture( PictureInfo_t **const picture, const Datetime_t *datetime,
                                              const WeatherCondition_t )
{
    static size_t  s_holidayPictureCounter = 0;
    static int32_t s_foundHolidayInfoIndex = NOT_FOUND;

    if ( s_foundHolidayInfoIndex == NOT_FOUND )
    {
        s_foundHolidayInfoIndex = tryToFindMatchingHoliday( datetime );

        if ( s_foundHolidayInfoIndex == NOT_FOUND )
            return NoAvailablePicture;
    }

    *picture = g_holidaysInfo[s_foundHolidayInfoIndex].pictures.data[s_holidayPictureCounter++];

    if ( g_holidaysInfo[s_foundHolidayInfoIndex].pictures.size <= s_holidayPictureCounter )
    {
        s_holidayPictureCounter = 0;
        s_foundHolidayInfoIndex = NOT_FOUND;
        return PictureGetterEndLoop;
    }

    return PictureGetterLoop;
}

PictureGetterLoopStatus_t getStandardModePicture( PictureInfo_t **const picture, const Datetime_t *,
                                                  const WeatherCondition_t )
{
    static size_t s_standardPictureIndex = 0;

    *picture = g_standardModePictures[s_standardPictureIndex++];

    if ( STANDARD_MODE_STATIC_PICTURE_COUNT <= s_standardPictureIndex )
    {
        s_standardPictureIndex = 0;
        return PictureGetterEndLoop;
    }

    return PictureGetterLoop;
}

PictureGetterLoopStatus_t callPictureGetterAtIndex( const size_t getterIndex, PictureInfo_t **const picture,
                                                    const Datetime_t        *datetime,
                                                    const WeatherCondition_t weatherCondition )
{
    return g_pictureGetterFunctions[getterIndex]( picture, datetime, weatherCondition );
}
