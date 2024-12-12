#include "wlan_adapter.h"
#include "wifi_cfg.h"

#include "logging.h"

#include "external/wlan/wlan_mwm.h"

#include <serial_mwm.h>


void initSerialMWM()
{
    LogInfo( "Initializing serial MWM" );
    int ret = mwm_init();
    if ( ret < 0 )
    {
        LogError( "Could not initialize Serial MWM, error: %d", ret );
        while ( 1 )
            ;
    }
    ret = wlan_get_state();
    if ( ret == MWM_INITIALIZED )
    {
        LogInfo( "Starting WLAN" );
        ret = mwm_wlan_start();
        if ( ret < 0 )
        {
            LogError( "Could not start WLAN subsystem, error: %d", ret );
            while ( 1 )
                ;
        }
    }
    wlan_config( AP_SSID, AP_PASSPHRASE, AP_SECURITY_MODE );

    int currentWlanState = wlan_get_state();
    while ( currentWlanState != MWM_CONNECTED )
    {
        if ( wlan_get_state() != currentWlanState )
            LogDebug( "WLAN didn't connect. Current state: %d", currentWlanState );

        currentWlanState = wlan_get_state();
    }

    wlan_state();
}
