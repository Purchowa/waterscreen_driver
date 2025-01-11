#include "waterscreen_config_parser.h"

#include "common_utils.h"

#include "cjson/cjson.h"


#define MAX_IDLE_TIME        30
#define MAX_WORK_TIME        30
#define MAX_WORK_RANGE_VALUE 24

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
    if ( !cJSON_IsNumber( mode ) || !( 0 <= mode->valueint && mode->valueint < CONFIG_AVAILABLE_MODE_COUNT ) )
        return Http_WaterscreenConfigParsingError;
    setWaterscreenMode( &config->mode, mode->valueint );

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

HttpReturnCodes_t parseJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config, bool isInitialRequest )
{
    cJSON                  *configJson  = cJSON_Parse( cfgJson );
    const HttpReturnCodes_t parsingCode = parseJsonConfig( configJson, config, isInitialRequest );
    cJSON_Delete( configJson );

    return parsingCode;
}
