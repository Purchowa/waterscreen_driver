#include "waterscreen_status_serializer.h"

#include "external/cjson/cJSON.h"

#define SERIALIZATION_ERROR NULL

static char *feedJsonObject( const WaterscreenStatus_t *status, cJSON *statusObj )
{
    char *dstString = NULL;

    if ( !statusObj )
        return SERIALIZATION_ERROR;

    const cJSON *mode = cJSON_AddNumberToObject( statusObj, "mode", status->mode );
    if ( !mode )
        return SERIALIZATION_ERROR;

    const cJSON *fluidLevel = cJSON_AddNumberToObject( statusObj, "fluidLevel", status->fluidLevel );
    if ( !fluidLevel )
        return SERIALIZATION_ERROR;

    const cJSON *isPresenting = cJSON_AddBoolToObject( statusObj, "isPresenting", status->isPresenting );
    if ( !isPresenting )
        return SERIALIZATION_ERROR;

    dstString = cJSON_PrintUnformatted( statusObj );
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
