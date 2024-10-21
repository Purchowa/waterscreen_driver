#include "waterscreen_config_parser.h"

#include "cjson/cjson.h"

#define MAX_IDLE_TIME 30
#define MAX_WORK_TIME 30

static uint16_t clamp_uint16( const int32_t value, const uint16_t min, const uint16_t max )
{
    const uint16_t lower = value < min ? min : value;
    return max < lower ? max : lower;
}

static HttpReturnCodes_t parseJsonConfig( const cJSON *cfgJson, WaterscreenConfig_t *config )
{
    if ( cfgJson == NULL )
        return Http_WaterscreenConfigParsingError;

    const cJSON *wasConfigRead = cJSON_GetObjectItemCaseSensitive( cfgJson, "wasRead" );
    if ( !cJSON_IsBool( wasConfigRead ) )
        return Http_WaterscreenConfigParsingError;

    if ( wasConfigRead->valueint )
        return Http_WaterscreenConfigNoUpdate;

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
    standardCfg->workTimeInStandardMode = clamp_uint16( workTime->valueint, 0, MAX_WORK_TIME );

    const cJSON *idleTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "idleTime" );
    if ( !cJSON_IsNumber( idleTime ) )
        return Http_WaterscreenConfigParsingError;
    standardCfg->idleTimeInStandardMode = clamp_uint16( idleTime->valueint, 0, MAX_IDLE_TIME );

    const cJSON *pictureObject = cJSON_GetObjectItemCaseSensitive( cfgJson, "picture" );
    if ( !cJSON_IsObject( pictureObject ) )
        return Http_WaterscreenConfigParsingError;

    const cJSON *pictureSize = cJSON_GetObjectItemCaseSensitive( pictureObject, "size" );
    if ( !cJSON_IsNumber( pictureSize ) )
        return Http_WaterscreenConfigParsingError;
    config->customPictureSize = clamp_uint16( pictureSize->valueint, 0, MAX_CUSTOM_PICTURE_LENGTH );

    const cJSON *pictureArray = cJSON_GetObjectItemCaseSensitive( pictureObject, "data" );
    if ( !cJSON_IsArray( pictureArray ) )
        return Http_WaterscreenConfigParsingError;

    const cJSON *element = NULL;
    uint16_t     i       = 0;
    cJSON_ArrayForEach( element, pictureArray )
    {
        if ( !cJSON_IsNumber( element ) )
            return Http_WaterscreenConfigParsingError;

        if ( config->customPictureSize <= i )
            break;

        config->customPicture[i++] = element->valueint;
    }

    if ( i < config->customPictureSize )
        return Http_WaterscreenConfigParsingError;

    return Http_Success;
}

HttpReturnCodes_t fromJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config )
{
    cJSON                  *configJson  = cJSON_Parse( cfgJson );
    const HttpReturnCodes_t parsingCode = parseJsonConfig( configJson, config );
    cJSON_Delete( configJson );

    return parsingCode;
}
