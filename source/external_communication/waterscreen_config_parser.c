#include "waterscreen_config_parser.h"

#include "cjson/cjson.h"

#define MAX_IDLE_TIME        30
#define MAX_WORK_TIME        30
#define MAX_WORK_RANGE_VALUE 24

#define MAX_COLOR_CHANNEL_VALUE 255

static int32_t clamp( const int32_t value, const int32_t min, const int32_t max )
{
    const int32_t lower = value < min ? min : value;
    return max < lower ? max : lower;
}

static bool parseRGBColor( const cJSON *obj, ColorRGB_t *color )
{
    const cJSON *r = cJSON_GetObjectItemCaseSensitive( obj, "r" );
    if ( !cJSON_IsNumber( r ) )
        return false;

    color->r = clamp( r->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    const cJSON *g = cJSON_GetObjectItemCaseSensitive( obj, "g" );
    if ( !cJSON_IsNumber( g ) )
        return false;

    color->g = clamp( g->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    const cJSON *b = cJSON_GetObjectItemCaseSensitive( obj, "b" );
    if ( !cJSON_IsNumber( b ) )
        return false;

    color->b = clamp( b->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    return true;
}

static bool parsePictureObject( const cJSON *pictureObject, PictureInfo_t *const customPicture )
{
    if ( !cJSON_IsObject( pictureObject ) )
        return false;

    const cJSON *pictureSize = cJSON_GetObjectItemCaseSensitive( pictureObject, "size" );
    if ( !cJSON_IsNumber( pictureSize ) )
        return false;
    customPicture->picture.size = clamp( pictureSize->valueint, 0, MAX_CUSTOM_PICTURE_HEIGHT );

    const cJSON *pictureArray = cJSON_GetObjectItemCaseSensitive( pictureObject, "data" );
    if ( !cJSON_IsArray( pictureArray ) )
        return false;

    const cJSON *element = NULL;
    uint16_t     i       = 0;
    cJSON_ArrayForEach( element, pictureArray )
    {
        if ( !cJSON_IsNumber( element ) )
            return false;

        if ( customPicture->picture.size <= i )
            break;

        customPicture->picture.data[i++] = element->valueint;
    }

    if ( i < customPicture->picture.size )
        return false;

    const cJSON *colorsObj = cJSON_GetObjectItemCaseSensitive( pictureObject, "colors" );
    if ( !cJSON_IsObject( colorsObj ) )
        return false;

    const cJSON *mainColorObj = cJSON_GetObjectItemCaseSensitive( colorsObj, "main" );
    if ( !cJSON_IsObject( mainColorObj ) )
        return false;

    if ( !parseRGBColor( mainColorObj, &customPicture->colors.main ) )
        return false;

    const cJSON *secondaryColorObj = cJSON_GetObjectItemCaseSensitive( colorsObj, "secondary" );
    if ( !cJSON_IsObject( secondaryColorObj ) )
        return false;

    if ( !parseRGBColor( secondaryColorObj, &customPicture->colors.secondary ) )
        return false;

    return true;
}

static HttpReturnCodes_t parseJsonConfig( const cJSON *cfgJson, WaterscreenConfig_t *config, bool isInitialRequest )
{
    if ( cfgJson == NULL )
        return Http_WaterscreenConfigParsingError;

    if ( !isInitialRequest )
    {
        const cJSON *wasConfigRead = cJSON_GetObjectItemCaseSensitive( cfgJson, "wasRead" );
        if ( !cJSON_IsBool( wasConfigRead ) )
            return Http_WaterscreenConfigParsingError;

        if ( wasConfigRead->valueint )
            return Http_WaterscreenConfigNoUpdate;
    }

    const cJSON *mode = cJSON_GetObjectItemCaseSensitive( cfgJson, "mode" );
    if ( !cJSON_IsNumber( mode ) || !( Mode_Standard <= mode->valueint && mode->valueint < Mode_SIZE ) )
        return Http_WaterscreenConfigParsingError;
    config->mode = mode->valueint;

    const cJSON *enableWeekends = cJSON_GetObjectItemCaseSensitive( cfgJson, "enableWeekends" );
    if ( !cJSON_IsBool( enableWeekends ) )
        return Http_WaterscreenConfigParsingError;

    StandardModeConfig_t *standardCfg    = &config->standardModeConfig;
    standardCfg->isWorkingDuringWeekends = enableWeekends->valueint;

    const cJSON *workTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "workTime" );
    if ( !cJSON_IsNumber( workTime ) )
        return Http_WaterscreenConfigParsingError;
    standardCfg->workTimeInStandardMode = clamp( workTime->valueint, 0, MAX_WORK_TIME );

    const cJSON *idleTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "idleTime" );
    if ( !cJSON_IsNumber( idleTime ) )
        return Http_WaterscreenConfigParsingError;
    standardCfg->idleTimeInStandardMode = clamp( idleTime->valueint, 0, MAX_IDLE_TIME );

    const cJSON *pictureObject = cJSON_GetObjectItemCaseSensitive( cfgJson, "picture" );
    if ( !parsePictureObject( pictureObject, config->customPicture ) )
        return Http_WaterscreenConfigParsingError;

    const cJSON *workRange = cJSON_GetObjectItemCaseSensitive( cfgJson, "workRange" );
    if ( !cJSON_IsObject( workRange ) )
        return Http_WaterscreenConfigParsingError;

    const cJSON *workRangeFrom = cJSON_GetObjectItemCaseSensitive( workRange, "from" );
    if ( !cJSON_IsNumber( workRangeFrom ) )
        return Http_WaterscreenConfigParsingError;

    const uint8_t workRangeFromValue = clamp( workRangeFrom->valueint, 0, MAX_WORK_RANGE_VALUE );

    const cJSON *workRangeTo = cJSON_GetObjectItemCaseSensitive( workRange, "to" );
    if ( !cJSON_IsNumber( workRangeTo ) )
        return Http_WaterscreenConfigParsingError;

    const uint8_t workRangeToValue = clamp( workRangeTo->valueint, 0, MAX_WORK_RANGE_VALUE );

    if ( workRangeToValue < workRangeFromValue )
        return Http_WaterscreenConfigParsingError;

    config->standardModeConfig.workRange.from = workRangeFromValue;
    config->standardModeConfig.workRange.to   = workRangeToValue;

    return Http_Success;
}

HttpReturnCodes_t fromJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config, bool isInitialRequest )
{
    cJSON                  *configJson  = cJSON_Parse( cfgJson );
    const HttpReturnCodes_t parsingCode = parseJsonConfig( configJson, config, isInitialRequest );
    cJSON_Delete( configJson );

    return parsingCode;
}
