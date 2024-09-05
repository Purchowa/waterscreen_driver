#include "waterscreen_config_parser.h"

#include "cjson/cjson.h"

#define MAX_IDLE_TIME 30
#define MAX_WORK_TIME 30

static uint16_t clamp_uint16( const int32_t value, const uint16_t min, const uint16_t max )
{
    const uint16_t lower = value < min ? min : value;
    return max < lower ? max : lower;
}

static bool parseJsonConfig( const cJSON *cfgJson, WaterscreenConfig_t *config )
{
    if ( cfgJson == NULL )
        return false;

    const cJSON *mode = cJSON_GetObjectItemCaseSensitive( cfgJson, "mode" );
    if ( !cJSON_IsNumber( mode ) || !( Standard <= mode->valueint && mode->valueint < SIZE ) )
        return false;
    config->mode = mode->valueint;

    const cJSON *enableWeekends = cJSON_GetObjectItemCaseSensitive( cfgJson, "enableWeekends" );
    if ( !cJSON_IsBool( enableWeekends ) )
        return false;
    config->isWorkingDuringWeekends = enableWeekends->valueint;

    const cJSON *workTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "workTime" );
    if ( !cJSON_IsNumber( workTime ) )
        return false;
    config->workTimeInStandardMode = clamp_uint16( workTime->valueint, 0, MAX_WORK_TIME );

    const cJSON *idleTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "idleTime" );
    if ( !cJSON_IsNumber( idleTime ) )
        return false;
    config->idleTimeInStandardMode = clamp_uint16( idleTime->valueint, 0, MAX_IDLE_TIME );

    const cJSON *pictureObject = cJSON_GetObjectItemCaseSensitive( cfgJson, "picture" );
    if ( !cJSON_IsObject( pictureObject ) )
        return false;

    const cJSON *pictureSize = cJSON_GetObjectItemCaseSensitive( pictureObject, "size" );
    if ( !cJSON_IsNumber( pictureSize ) )
        return false;
    config->customPictureSize = clamp_uint16( pictureSize->valueint, 0, MAX_CUSTOM_PICTURE_LENGTH );

    const cJSON *pictureArray = cJSON_GetObjectItemCaseSensitive( pictureObject, "data" );
    if ( !cJSON_IsArray( pictureArray ) )
        return false;

    const cJSON *element = NULL;
    uint16_t     i       = 0;
    cJSON_ArrayForEach( element, pictureArray )
    {
        if ( !cJSON_IsNumber( element ) )
            return false;

        if ( config->customPictureSize <= i )
            break;

        config->customPicture[i++] = element->valueint;
    }

    if ( i < config->customPictureSize )
        return false;

    return true;
}

bool fromJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config )
{
    cJSON     *configJson            = cJSON_Parse( cfgJson );
    const bool wasSuccessfullyParsed = parseJsonConfig( configJson, config );
    cJSON_Delete( configJson );

    return wasSuccessfullyParsed;
}
