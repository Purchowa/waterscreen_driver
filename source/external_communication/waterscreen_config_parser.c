#include "waterscreen_config_parser.h"

#include "cjson/cjson.h"

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
    if ( !cJSON_IsNumber( workTime ) || workTime->valueint < 0 )
        return false;
    config->workTimeInStandardMode = workTime->valueint;

    const cJSON *idleTime = cJSON_GetObjectItemCaseSensitive( cfgJson, "idleTime" );
    if ( !cJSON_IsNumber( idleTime ) || idleTime->valueint < 0 )
        return false;
    config->idleTimeInStandardMode = idleTime->valueint;

    const cJSON *pictureObject = cJSON_GetObjectItemCaseSensitive( cfgJson, "picture" );
    if ( !cJSON_IsObject( pictureObject ) )
        return false;

    const cJSON *pictureSize = cJSON_GetObjectItemCaseSensitive( pictureObject, "size" );
    if ( !cJSON_IsNumber( pictureSize ) ||
         ( pictureSize->valueint < 0 || MAX_CUSTOM_PICTURE_LENGTH < pictureSize->valueint ) )
        return false;
    config->customPictureSize = pictureSize->valueint;

    const cJSON *pictureArray = cJSON_GetObjectItemCaseSensitive( pictureObject, "data" );
    if ( !cJSON_IsArray( pictureArray ) )
        return false;

    const cJSON *element = NULL;
    uint16_t     i       = 0;
    cJSON_ArrayForEach( element, pictureArray )
    {
        if ( !cJSON_IsNumber( element ) || config->customPictureSize <= i )
            return false;

        config->customPicture[i++] = element->valueint;
    }

    return true;
}

bool loadFromRawJsonIntoConfig( const char *cfgJson, WaterscreenConfig_t *config )
{
    cJSON     *configJson            = cJSON_Parse( cfgJson );
    const bool wasSuccessfullyParsed = parseJsonConfig( configJson, config );
    cJSON_Delete( configJson );

    return wasSuccessfullyParsed;
}
