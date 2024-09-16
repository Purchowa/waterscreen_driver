#include "picture_logic.h"

#include "picture_data.h"

#include <string.h>
#include <assert.h>


const PictureDataView_t *getEachPictureView()
{
    static size_t s_pictureIndex = 0;

    assert( s_pictureIndex < PICTURE_COUNT );
    const PictureDataView_t *currentPictureViewPtr = &g_allPictures[s_pictureIndex];
    s_pictureIndex                                 = ( s_pictureIndex + 1 ) % PICTURE_COUNT;

    return currentPictureViewPtr;
}

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

static const PictureDataView_t *getCurrentTimeAsPicture( const Datetime_t *datetime, const WeatherCondition_t )
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

    return &s_timePictureView;
}

static const PictureDataView_t *getWeatherAsPicture( const Datetime_t *, const WeatherCondition_t weatherCondition )
{
    return &g_weatherPictures[weatherCondition];
}

static const PictureDataView_t *getStandardModePicture( const Datetime_t *, const WeatherCondition_t )
{
    static size_t s_standardPictureIndex = 0;

    assert( s_standardPictureIndex < STANDARD_MODE_STATIC_PICTURE_COUNT );
    const PictureDataView_t *standardPictureViewPtr = &g_standardModePictures[s_standardPictureIndex];

    s_standardPictureIndex = ( s_standardPictureIndex + 1 ) % STANDARD_MODE_STATIC_PICTURE_COUNT;

    return standardPictureViewPtr;
}

typedef const PictureDataView_t *( *pictureGetterFun_t )( const Datetime_t *, const WeatherCondition_t );

static const pictureGetterFun_t s_pictureGetterFunctions[STANDARD_MODE_PICTURE_GETTER_COUNT] = {
    getStandardModePicture, getWeatherAsPicture,    getCurrentTimeAsPicture, getStandardModePicture,
    getStandardModePicture, getStandardModePicture, getStandardModePicture,  getStandardModePicture,
    getStandardModePicture, getCurrentTimeAsPicture };

const PictureDataView_t *getOccasionalPictureView( const Datetime_t        *datetime,
                                                   const WeatherCondition_t weatherCondition )
{
    static size_t s_callCounter = 0;

    assert( s_callCounter < STANDARD_MODE_PICTURE_GETTER_COUNT );
    const pictureGetterFun_t pictureGetterFun = s_pictureGetterFunctions[s_callCounter];

    s_callCounter = ( s_callCounter + 1 ) % STANDARD_MODE_PICTURE_GETTER_COUNT;

    return pictureGetterFun( datetime, weatherCondition );
}

static uint64_t s_userCustomPicture[MAX_CUSTOM_PICTURE_LENGTH];

CustomPictureDataSpan_t *getCustomPictureSpan()
{
    static CustomPictureDataSpan_t view = {
        .capacity = MAX_CUSTOM_PICTURE_LENGTH, .size = 0, .data = s_userCustomPicture };

    return &view;
}

uint8_t getLastPictureIndex( const PictureDataView_t *picture )
{
    assert( picture );

    return picture->size - 1;
}
