/*
 * wlan_mwm.c
 *
 *  Created on: 19 lut 2022
 *      Author: daniel
 */
#include "wlan_mwm.h"

#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "serial_mwm.h"
#include "logging.h"

#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_WLAN_SCAN  5
#define MWM_BUFFER_LEN 2048
#define RXD_BUFFER_LEN 1400
#define TXD_BUFFER_LEN 256
#define HOSTNAME_LEN   128


/*******************************************************************************
 * Variables
 ******************************************************************************/

static mwm_sockaddr_t g_http_srv_addr = { .host = "", .port = 80 };
static mwm_wlan_t     g_wlans[MAX_WLAN_SCAN];
static char           g_buffer[MWM_BUFFER_LEN];

/*******************************************************************************
 * Code
 ******************************************************************************/

int wlan_get_state()
{
    int ret = mwm_wlan_status();
    if ( ret < 0 )
    {
        LogError( "Failed to get WLAN status, error: %d", ret );
    }

    return ret;
}

void wlan_state()
{

    PRINTF( "WLAN State: " );

    int ret = wlan_get_state();
    if ( ret == MWM_INITIALIZED )
    {
        PRINTF( "Initialized\r\n" );
    }
    else if ( ret == MWM_CONNECTED )
    {
        char ssid[33]    = { 0 };
        char ip_addr[16] = { 0 };
        ret              = mwm_wlan_info( ssid, ip_addr );
        if ( ret == 0 )
        {
            PRINTF( "Connected\r\nSSID: %s\r\nIP Address: %s\r\n", ssid, ip_addr );
        }
        else
        {
            PRINTF( "Connected\r\n" );
        }
    }
    else if ( ret == MWM_CONNECTING )
    {
        ret = mwm_get_param( MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, MWM_BUFFER_LEN );
        if ( ret < 0 )
        {
            PRINTF( "Connecting...\r\n" );
        }
        else
        {
            PRINTF( "Connecting to %s\r\n", g_buffer );
        }
    }
    else if ( ret == MWM_AUTH_FAILED )
    {
        PRINTF( "Connection failed, Wi-Fi authentication failed.\r\n" );
    }
    else if ( ret == MWM_NETWORK_NOT_FOUND )
    {
        PRINTF( "Connection failed, WLAN not found.\r\n" );
    }
    else if ( ret == MWM_PROVISIONING )
    {
        ret = mwm_get_param( MWM_MOD_PROV, MWM_PROV_SSID, g_buffer, MWM_BUFFER_LEN );
        if ( ret < 0 )
        {
            PRINTF( "Could not get param: %s, error: %d\r\n", MWM_PROV_SSID, ret );
            while ( 1 )
                ;
        }
        PRINTF( "Provisioning mode\r\n\r\n" );
        PRINTF( "For configuration by web application: connect to WLAN: %s and open address: http://192.168.10.1 "
                "in web browser.\r\n",
                g_buffer );
        PRINTF( "For configuration by this application select from menu: '4 - WLAN Configuration - Client'\r\n\r\n" );
    }
    else if ( ret == MWM_DISCONNECTED )
    {
        PRINTF( "Disconnected\r\n" );
    }
    else
    {
        PRINTF( "%d\r\n", ret );
    }
    PRINTF( "--------------------------------\r\n" );
}

void wlan_scan()
{
    int ret;

    PRINTF( "Scanning...\r\n" );
    ret = mwm_wlan_scan( g_wlans, MAX_WLAN_SCAN );
    if ( ret < 0 )
    {
        PRINTF( "WLAN scan failed, error: %d\r\n", ret );
        while ( 1 )
            ;
    }

    if ( ret == 0 )
    {
        PRINTF( "WLANs not found\r\n" );
        return;
    }

    int i;
    PRINTF( "\r\n" );
    for ( i = 0; i < ret; i++ )
    {
        PRINTF( "SSID: %s\r\n", g_wlans[i].ssid );
        PRINTF( "Channel: %d\r\n", g_wlans[i].channel );
        PRINTF( "RSSI: %d\r\n", g_wlans[i].rssi );
        PRINTF( "Security: %d\r\n", g_wlans[i].security );
        PRINTF( "\r\n" );
    }
}

void wlan_connect()
{
    int ret;

    ret = wlan_get_state();
    if ( ret != MWM_DISCONNECTED )
    {
        LogDebug( "WLAN must be in disconnected state" );
        return;
    }

    ret = mwm_get_param( MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, MWM_BUFFER_LEN );
    if ( ret < 0 )
    {
        LogInfo( "Connecting..." );
    }
    else
    {
        LogInfo( "Connecting to %s", g_buffer );
    }

    ret = mwm_wlan_connect();
    if ( ret < 0 )
    {
        LogError( "WLAN connect failed, error: %d", ret );
    }
}

void wlan_disconnect()
{

    int ret;

    ret = wlan_get_state();
    if ( ret != MWM_CONNECTED )
    {
        PRINTF( "WLAN must be in connected state\r\n" );
        return;
    }

    PRINTF( "Disconnecting...\r\n" );
    ret = mwm_wlan_disconnect();
    if ( ret < 0 )
    {
        PRINTF( "WLAN disconnect failed, error: %d\r\n", ret );
        while ( 1 )
            ;
    }
}

void wlan_reboot()
{

    int ret;

    LogInfo( "Rebooting WLAN module..." );
    ret = mwm_reboot();
    if ( ret < 0 )
    {
        LogError( "WLAN reboot failed, error: %d", ret );
        while ( 1 )
            ;
    }

    LogInfo( "Starting WLAN..." );
    ret = mwm_wlan_start();
    if ( ret < 0 )
    {
        LogError( "Could not start WLAN subsystem, error: %d", ret );
        while ( 1 )
            ;
    }
}

void wlan_config( char *ap_ssid, char *ap_passphrase, char *ap_security_mode )
{

    int  ret;
    bool configured = false;
    bool recofigure = false;

    ret = mwm_get_param( MWM_MOD_WLAN, MWM_WLAN_CONFIGURED, g_buffer, MWM_BUFFER_LEN );
    if ( ret < 0 )
    {
        LogError( "Could not get param: %s, error: %d", MWM_PROV_SSID, ret );
        while ( 1 )
            ;
    }

    if ( strcmp( g_buffer, "1" ) == 0 )
    {
        /* WLAN is configured */
        ret = mwm_get_param( MWM_MOD_WLAN, MWM_WLAN_SSID, g_buffer, MWM_BUFFER_LEN );
        if ( ret < 0 )
        {
            LogError( "Could not get param: %s, error: %d", MWM_PROV_SSID, ret );
            while ( 1 )
                ;
            ;
        }

        if ( strcmp( g_buffer, ap_ssid ) )
            recofigure = true;

        ret = mwm_get_param( MWM_MOD_WLAN, MWM_WLAN_SECURITY, g_buffer, MWM_BUFFER_LEN );
        if ( ret < 0 )
        {
            LogError( "Could not get param: %s, error: %d", MWM_PROV_SSID, ret );
            while ( 1 )
                ;
        }

        if ( strcmp( g_buffer, ap_security_mode ) )
            recofigure = true;

        configured = true;
    }

    if ( recofigure )
    {

        LogInfo( "Reconfigure WLAN module" );

        ret = mwm_set_param( MWM_MOD_WLAN, MWM_WLAN_SSID, ap_ssid );
        if ( ret < 0 )
        {
            LogError( "Could not set param: %s, error: %d", MWM_WLAN_SSID, ret );
            while ( 1 )
                ;
        }

        ret = mwm_set_param( MWM_MOD_WLAN, MWM_WLAN_SECURITY, ap_security_mode );
        if ( ret < 0 )
        {
            LogError( "Could not set param: %s, error: %d", MWM_WLAN_SSID, ret );
            while ( 1 )
                ;
        }

        if ( g_buffer[0] > '1' )
        {
            /* Security is not open - passphrase is required */
            ret = mwm_set_param( MWM_MOD_WLAN, MWM_WLAN_PASSPHRASE, ap_passphrase );
            if ( ret < 0 )
            {
                LogError( "Could not set param: %s, error: %d", MWM_WLAN_SSID, ret );
                while ( 1 )
                    ;
            }
        }

        if ( configured == false )
        {
            ret = mwm_set_param( MWM_MOD_WLAN, MWM_WLAN_CONFIGURED, "1" );
            if ( ret < 0 )
            {
                LogError( "Could not set param: %s, error: %d", MWM_WLAN_CONFIGURED, ret );
                while ( 1 )
                    ;
            }
        }

        wlan_reboot();
    }
}

void wlan_init( char *ap_ssid, char *ap_passphrase, char *ap_security_mode )
{

    int ret;

    /* Initialize Serial MWM */
    LogInfo( "Initializing Serial MWM..." );
    ret = mwm_init();
    if ( ret < 0 )
    {

        LogError( "Could not initialize Serial MWM, error: %d", ret );
        while ( 1 )
            ;
    }
    ret = wlan_get_state();
    if ( ret == MWM_INITIALIZED )
    {

        LogInfo( "WLAN Starting..." );
        ret = mwm_wlan_start();
        if ( ret < 0 )
        {

            LogError( "Could not start WLAN subsystem, error: %d", ret );
            while ( 1 )
                ;
        }
    }

    wlan_config( ap_ssid, ap_passphrase, ap_security_mode );
    LogInfo( "WLAN Connecting..." );
}

static void closeSocket( const int socketHandle )
{
    int ret = mwm_close( socketHandle );
    if ( ret < 0 )
    {
        LogError( "Could not close socket, error: %d", ret );
        wlan_reboot();
    }
}

void http_POST( const char *reqdata, const char *authorization, char *response_data, const uint32_t response_data_len,
                char *content_data )
{

    char hostname[HOSTNAME_LEN] = { 0 };

    for ( int i = 0; ( ( reqdata[i] != '/' ) && ( reqdata[i] != '\0' ) && i < HOSTNAME_LEN ); i++ )
    {

        hostname[i] = reqdata[i];
    }

    int ret;
    int s;
    int data_len;

    ret = wlan_get_state();
    if ( ret != MWM_CONNECTED )
    {

        LogDebug( "WLAN must be in connected state" );
        return;
    }

    s = mwm_socket( MWM_TCP );
    if ( s < 0 )
    {

        LogError( "Could not create socket, error: %d", ret );
        return;
    }

    sprintf( g_http_srv_addr.host, hostname );
    g_http_srv_addr.port = 80;

    ret = mwm_connect( s, &g_http_srv_addr, sizeof( g_http_srv_addr ) );
    if ( ret != 0 )
    {

        LogError( "Could not connect to server, error: %d", ret );
        closeSocket( s );
        return;
    }

    char request[TXD_BUFFER_LEN];

    if ( authorization )
    {
        sprintf( request,
                 "POST http://%s HTTP/1.0\r\n"
                 "Authorization:%s\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: %d\r\n\r\n"
                 "%s\r\n",
                 reqdata, authorization, hostname, strlen( content_data ), content_data );
    }
    else
    {
        sprintf( request,
                 "POST http://%s HTTP/1.0\r\n"
                 "Host: %s\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: %d\r\n\r\n"
                 "%s\r\n",
                 reqdata, hostname, strlen( content_data ), content_data );
    }


    data_len = strlen( request );
    ret      = mwm_send( s, request, data_len );
    if ( ret != data_len )
    {

        LogError( "Could not send data, error: %d", ret );
        closeSocket( s );
        return;
    }

    ret = mwm_recv_timeout( s, response_data, response_data_len, 0 );

    response_data[ret] = '\0';

    closeSocket( s );
}

void http_GET( const char *reqdata, const char *authorization, char *respdata )
{
    char hostname[HOSTNAME_LEN] = { 0 };

    for ( int i = 0; ( ( reqdata[i] != '/' ) && ( reqdata[i] != '\0' ) && i < HOSTNAME_LEN ); i++ )
    {

        hostname[i] = reqdata[i];
    }

    int ret;
    int s;
    int data_len;

    ret = wlan_get_state();
    if ( ret != MWM_CONNECTED )
    {

        LogError( "WLAN must be in connected state" );
        return;
    }

    s = mwm_socket( MWM_TCP );
    if ( s < 0 )
    {

        LogError( "Could not create socket, error: %d", ret );
        return;
    }

    sprintf( g_http_srv_addr.host, hostname );
    g_http_srv_addr.port = 80;

    // PRINTF( "Connecting to: %s:%d\r\n", g_http_srv_addr.host, g_http_srv_addr.port );

    ret = mwm_connect( s, &g_http_srv_addr, sizeof( g_http_srv_addr ) );
    if ( ret != 0 )
    {

        LogError( "Could not connect to server, error: %d", ret );
        closeSocket( s );
        return;
    }

    char request[256];

    if ( authorization )
        sprintf( request, "GET http://%s HTTP/1.0\r\nAuthorization: %s\r\nHost: %s\r\n\r\n", reqdata, authorization,
                 hostname );
    else
        sprintf( request, "GET http://%s HTTP/1.0\r\nHost: %s\r\n\r\n", reqdata, hostname );

    data_len = strlen( request );
    ret      = mwm_send( s, request, data_len );
    if ( ret != data_len )
    {

        LogError( "Could not send data, error: %d\r\n", ret );
        closeSocket( s );
        return;
    }

    ret           = mwm_recv_timeout( s, respdata, RXD_BUFFER_LEN, 0 );
    respdata[ret] = '\0';

    closeSocket( s );
}

void http_head_parser( const char *headdata, char *parseval, char *keyval )
{

    char *parse = 0;
    int   i;

    parse = strstr( headdata, keyval );
    if ( parse )
    {

        parse += strlen( keyval );
        for ( i = 0; ( parse[i] != '\r' ) && ( parse[i] != '\n' ); i++ )
            parseval[i] = parse[i];
        parseval[i] = 0;
    }
}
