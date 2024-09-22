#include "standard_mode_picture_logic.h"

#include "picture_data.h"

#include <string.h>
#include <assert.h>


typedef enum
{
    PictureGetterLoop,
    PictureGetterEndLoop
} PictureGetterLoopStatus_t;

static PictureDataView_t s_timePictureView = { .size = CHARACTER_TO_PICTURE_ROW_COUNT, .data = NULL };

#define ONES_EXTRACTOR 10
static uint8_t extractOnesFromNumber( const uint8_t number )
{
    return number % ONES_EXTRACTOR;
}

#define TENS_EXTRACTOR 100
static uint8_t extractTensFromNumber( const uint8_t number )
{
    return ( number / ONES_EXTRACTOR ) % TENS_EXTRACTOR;
}

static PictureGetterLoopStatus_t getCurrentTimeAsPicture( const PictureDataView_t **const picture,
                                                          const Datetime_t *datetime, const WeatherCondition_t )
{
    memset( g_timePicture, 0, sizeof( *g_timePicture ) * CHARACTER_TO_PICTURE_ROW_COUNT );

    uint8_t hourOnes = extractOnesFromNumber( datetime->time.hour );
    uint8_t hourTens = extractTensFromNumber( datetime->time.hour );

    uint8_t minuteOnes = extractOnesFromNumber( datetime->time.minute );
    uint8_t minuteTens = extractTensFromNumber( datetime->time.minute );

    assert( hourOnes < CHARACTER_TO_PICTURE_MAP_SIZE && hourTens < CHARACTER_TO_PICTURE_MAP_SIZE &&
            minuteOnes < CHARACTER_TO_PICTURE_MAP_SIZE && minuteTens < CHARACTER_TO_PICTURE_MAP_SIZE );

    for ( size_t i = 0; i < CHARACTER_TO_PICTURE_ROW_COUNT; ++i )
    {
        g_timePicture[i] = (pictureRow_t)g_characterToPictureMap[hourTens][i] << HOUR_TENS_SHIFT |
            (pictureRow_t)g_characterToPictureMap[hourOnes][i] << HOUR_ONES_SHIFT |
            (pictureRow_t)g_characterToPictureMap[ColonIdx][i] << COLON_SHIFT |
            (pictureRow_t)g_characterToPictureMap[minuteTens][i] << MINUTE_TENS_SHIFT |
            (pictureRow_t)g_characterToPictureMap[minuteOnes][i] >> MINUTE_ONES_SHIFT;
    }

    s_timePictureView.data = g_timePicture;

    *picture = &s_timePictureView;

    return PictureGetterEndLoop;
}

static PictureGetterLoopStatus_t getWeatherAsPicture( const PictureDataView_t **const picture, const Datetime_t *,
                                                      const WeatherCondition_t        weatherCondition )
{
    *picture = &g_weatherPictures[weatherCondition];

    return PictureGetterEndLoop;
}

// static PictureGetterLoopStatus_t getSeasonalPicture( const Datetime_t *datetime, const WeatherCondition_t ) {}

static PictureGetterLoopStatus_t getStandardModePicture( const PictureDataView_t **const picture, const Datetime_t *,
                                                         const WeatherCondition_t )
{
    static size_t s_standardPictureIndex = 0;

    assert( s_standardPictureIndex < STANDARD_MODE_STATIC_PICTURE_COUNT );
    *picture = &g_standardModePictures[s_standardPictureIndex++];

    if ( STANDARD_MODE_STATIC_PICTURE_COUNT <= s_standardPictureIndex )
    {
        s_standardPictureIndex = 0;
        return PictureGetterEndLoop;
    }

    return PictureGetterLoop;
}

typedef PictureGetterLoopStatus_t ( *pictureGetterFun_t )( const PictureDataView_t **const, const Datetime_t *,
                                                           const WeatherCondition_t );

static const pictureGetterFun_t s_pictureGetterFunctions[STANDARD_MODE_PICTURE_GETTER_COUNT] = {
    getWeatherAsPicture, getCurrentTimeAsPicture, getStandardModePicture, getCurrentTimeAsPicture };

const PictureDataView_t *getOccasionalPictureView( const Datetime_t        *datetime,
                                                   const WeatherCondition_t weatherCondition )
{
    static size_t s_callCounter = 0;

    assert( s_callCounter < STANDARD_MODE_PICTURE_GETTER_COUNT );
    const pictureGetterFun_t pictureGetterFun = s_pictureGetterFunctions[s_callCounter];

    const PictureDataView_t        *pictureView  = NULL;
    const PictureGetterLoopStatus_t getterStatus = pictureGetterFun( &pictureView, datetime, weatherCondition );

    if ( getterStatus == PictureGetterEndLoop )
    {
        s_callCounter = ( s_callCounter + 1 ) % STANDARD_MODE_PICTURE_GETTER_COUNT;
    }

    return pictureView;
}
