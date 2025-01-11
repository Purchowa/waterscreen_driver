#include "waterscreen_config_api.h"

#include "waterscreen_config_parser.h"

#include "config/wifi_cfg.h"
#include "wlan/wlan_mwm.h"
#include "http_return_codes.h"
#include "http_utils.h"

#include <stdlib.h>
#include <assert.h>

HttpReturnCodes_t httpGetWaterscreenConfig( WaterscreenConfig_t *config, bool isInitialRequest )
{
    char      *configContent = NULL;
    const bool wasReceived =
        httpGET_receiveContent( &configContent, WATERSCREEN_API_BASIC_AUTH_BASE64, WATERSCREEN_CONFIG_GET_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( configContent );
    return parseJsonToWaterscreenCfg( configContent, config, isInitialRequest );
}
