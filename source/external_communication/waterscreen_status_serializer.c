#include "waterscreen_status_serializer.h"

#include "external/cjson/cJSON.h"

#define SERIALIZATION_ERROR NULL

static char *feedJsonObject( const WaterscreenStatus_t *status, cJSON *statusObj )
{
    char *dstString = NULL;

    if ( !statusObj )
        return SERIALIZATION_ERROR;

    cJSON *mode = cJSON_CreateNumber( status->mode );
    if ( !mode )
        return SERIALIZATION_ERROR;
    cJSON_AddItemToObject( statusObj, "mode", mode );

    cJSON *fluidLevel = cJSON_CreateNumber( status->fluidLevel );
    if ( !fluidLevel )
        return SERIALIZATION_ERROR;
    cJSON_AddItemToObject( statusObj, "fluidLevel", fluidLevel );

    cJSON *isPresenting = cJSON_CreateBool( status->isPresenting );
    if ( !isPresenting )
        return SERIALIZATION_ERROR;
    cJSON_AddItemToObject( statusObj, "isPresenting", isPresenting );

    dstString = cJSON_Print( statusObj );
    if ( !dstString )
        return SERIALIZATION_ERROR;

    return dstString;
}

char *serializeWaterscreenStatus( const WaterscreenStatus_t *status )
{
    cJSON *statusObj  = cJSON_CreateObject();
    char  *jsonString = feedJsonObject( status, statusObj );
    cJSON_Delete( statusObj );

    return jsonString;
}
