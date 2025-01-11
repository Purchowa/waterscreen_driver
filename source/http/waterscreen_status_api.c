#include "waterscreen_status_api.h"
#include "waterscreen_status_serializer.h"
#include "http_utils.h"
#include "config/wifi_cfg.h"

#include "external/wlan/wlan_mwm.h"

#include <stdlib.h>
#include <string.h>

#define HTTP_POST_RESPONSE_LEN 512

static char s_postResponse[HTTP_POST_RESPONSE_LEN];


HttpReturnCodes_t httpPostWaterscreenStatus( const WaterscreenStatus_t *status )
{
    char *statusJsonStr = serializeWaterscreenStatus( status );

    HttpReturnCodes_t returnCode = Http_Success;
    if ( !statusJsonStr )
        returnCode = Http_WaterscreenStatusSerializationError;

    memset( s_postResponse, '\0', HTTP_POST_RESPONSE_LEN );
    http_POST( WATERSCREEN_STATUS_POST_URL, WATERSCREEN_API_BASIC_AUTH_BASE64, s_postResponse, HTTP_POST_RESPONSE_LEN,
               statusJsonStr );

    long       httpCode;
    const bool wasExtracted = extractHttpResponseCode( s_postResponse, &httpCode );
    if ( !wasExtracted || httpCode != HTTP_CODE_OK )
        returnCode = Http_POSTError;

    free( statusJsonStr );
    return returnCode;
}
