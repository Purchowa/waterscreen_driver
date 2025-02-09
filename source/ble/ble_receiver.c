#include "ble_receiver.h"

#include "common_utils.h"
#include "receiver_handlers.h"
#include "receiver_data_types.h"

#include "picture_management/picture_types.h"
#include "picture_management/picture_data.h"
#include "config/rtos_time_defines.h"
#include "config/wifi_cfg.h"
#include "http/wifi_task.h"

#include "status_logging/status_logging.h"
#include "logging.h"

#include <inttypes.h>
#include <task.h>

#define BLE_RECEIVE_BUFFER_CAPACITY 8192

#define MAX_DATA_DELAY_TICKS pdMS_TO_TICKS( 5 * SECOND_MS )


static ColorRGB_t receiveRGB( SerializedColorRGB_t c )
{
    ColorRGB_t rgb = { c.r, c.g, c.b };

    return rgb;
}
static void receivePictureColors( const SerializedPictureColors_t *serializedColors, PictureColors_t *colors )
{
    colors->main      = receiveRGB( serializedColors->main );
    colors->secondary = receiveRGB( serializedColors->secondary );
}


StreamBufferHandle_t g_rxBLEBuffer     = NULL;
static bool          s_isLoggingActive = false;

static void onDataReceiveFailure( const char *dataType )
{
    xStreamBufferReset( g_rxBLEBuffer );
    LogError( "Some data might got lost while receiving %s", dataType );
}

static void onDataReceiveSuccess( const char *dataType )
{
    LogInfo( "Got %s", dataType );
}

void bleReceiverTask( void * )
{
    g_rxBLEBuffer = xStreamBufferCreate( BLE_RECEIVE_BUFFER_CAPACITY, sizeof( typeInfo_t ) );
    if ( !g_rxBLEBuffer )
    {
        LogError( "Not enough heap memory to allocate BLE buffer!" );
        vTaskSuspend( NULL );
    }

    typeInfo_t typeInfo = 0;
    for ( ;; )
    {
        // { 1B[typeInfo]|xB[rest...] }
        xStreamBufferReceive( g_rxBLEBuffer, &typeInfo, sizeof( typeInfo_t ), portMAX_DELAY );

        bool isOk = true;
        switch ( typeInfo )
        {
        case RTModeActive: // { 1B[isRTModeActive] }
            {
                bool isRTModeActive = false;
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( rtModeActive_t ) );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &isRTModeActive, sizeof( rtModeActive_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( rtModeActive_t );

                if ( !isOk )
                {
                    onDataReceiveFailure( "RTModeActive" );
                    break;
                }
                onDataReceiveSuccess( "RTModeActive" );

                handleBLERTModeEvent( isRTModeActive );
                break;
            }

        case CustomPicture: // { 6B[colors-main,secondary]|4B[pictureSize]|size_B[pictureData] }
            {
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer,
                                              sizeof( SerializedPictureColors_t ) + sizeof( pictureSize_t ) );

                SerializedPictureColors_t colorsPayload;

                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &colorsPayload, sizeof( SerializedPictureColors_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( SerializedPictureColors_t );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &g_customPicture.picture.size, sizeof( pictureSize_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( pictureSize_t );

                g_customPicture.picture.size = clamp( g_customPicture.picture.size, 0, MAX_CUSTOM_PICTURE_HEIGHT );

                const size_t expectedPictureSize = g_customPicture.picture.size * sizeof( pictureRow_t );
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, expectedPictureSize );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, g_customPicture.picture.data, expectedPictureSize,
                                              MAX_DATA_DELAY_TICKS ) == expectedPictureSize;

                if ( !isOk )
                {
                    onDataReceiveFailure( "CustomPicture" );
                    break;
                }
                onDataReceiveSuccess( "CustomPicture" );

                receivePictureColors( &colorsPayload, &g_customPicture.colors );
                handleCustomPictureEvent();
                break;
            }
        case WiFiCredentials: // { 1B[loginSize]|size_B[login]|1B[passwordSize]|size_B[password] }
            {
                wifiCredentialsSize_t loginSize    = 0;
                wifiCredentialsSize_t passwordSize = 0;

                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( wifiCredentialsSize_t ) );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &loginSize, sizeof( wifiCredentialsSize_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( wifiCredentialsSize_t );
                loginSize = clamp( loginSize, 0, WIFI_CREDENTIALS_CAPACITY );

                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, loginSize );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, g_wifiCredentials.login, loginSize,
                                              MAX_DATA_DELAY_TICKS ) == loginSize;
                g_wifiCredentials.login[WIFI_CREDENTIALS_CAPACITY - 1] = '\0';

                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( wifiCredentialsSize_t ) );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &passwordSize, sizeof( wifiCredentialsSize_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( wifiCredentialsSize_t );

                passwordSize = clamp( passwordSize, 0, WIFI_CREDENTIALS_CAPACITY );
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, passwordSize );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, g_wifiCredentials.password, passwordSize,
                                              MAX_DATA_DELAY_TICKS ) == passwordSize;

                g_wifiCredentials.password[WIFI_CREDENTIALS_CAPACITY - 1] = '\0';

                if ( !isOk )
                {
                    onDataReceiveFailure( "WiFiCredentials" );
                    break;
                }
                onDataReceiveSuccess( "WiFiCredentials" );

                reconfigureWifi();
                break;
            }
        case LogsActive: // { 1B[areLogsActive] }
            {
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( logsActive_t ) );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &s_isLoggingActive, sizeof( logsActive_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( logsActive_t );

                if ( !isOk )
                {
                    onDataReceiveFailure( "LogsActive" );
                    break;
                }
                onDataReceiveSuccess( "LogsActive" );

                break;
            }

        case Configuration:
            {
                SerializedConfiguration_t config = {};
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( SerializedConfiguration_t ) );
                isOk &= xStreamBufferReceive( g_rxBLEBuffer, &config, sizeof( SerializedConfiguration_t ),
                                              MAX_DATA_DELAY_TICKS ) == sizeof( SerializedConfiguration_t );

                if ( !isOk )
                {
                    onDataReceiveFailure( "Configuration" );
                    break;
                }
                onDataReceiveSuccess( "Configuration" );

                handleConfiguration( &config );
                logWaterscreenConfig( &g_waterscreenConfig );
                break;
            }
        default:
            {
                LogDebug( "Unknown incoming data" );
                xStreamBufferReset( g_rxBLEBuffer );
            }
        }
        xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( typeInfo_t ) );
    }
}

bool isLoggingActive()
{
    return s_isLoggingActive;
}
