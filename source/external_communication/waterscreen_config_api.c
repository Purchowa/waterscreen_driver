#include "waterscreen_config_api.h"

#include "waterscreen_config_parser.h"

#include "config/wifi_cfg.h"
#include "wlan/wlan_mwm.h"
#include "http_return_codes.h"
#include "http_get_utils.h"

#include <stdlib.h>
#include <assert.h>

HttpReturnCodes_t getWaterscreenConfigFromApi( WaterscreenConfig_t *config )
{
    char      *configContent = NULL;
    const bool wasReceived =
        httpGET_receiveContent( &configContent, WATERSCREEN_CONFIG_BASIC_AUTH_BASE64, WATERSCREEN_CONFIG_API_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( configContent );
    return fromJsonToWaterscreenCfg( configContent, config );
}
