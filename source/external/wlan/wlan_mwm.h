/*
 * wlan_mwm.h
 *
 *  Created on: 19 lut 2022
 *      Author: daniel
 */

#ifndef WLAN_MWM_H_
#define WLAN_MWM_H_

#include <stdint.h>

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int  wlan_get_state();
void wlan_state();
void wlan_scan();
void wlan_connect();
void wlan_disconnect();
void wlan_reboot();
void wlan_config( char *ap_ssid, char *ap_passphrase, char *ap_security_mode );
void wlan_init( char *ap_ssid, char *ap_passphrase, char *ap_security_mode );
void http_POST( const char *reqdata, const char *authorization, char *response_data, const uint32_t response_data_len,
                char *content_data );
void http_GET( const char *reqdata, const char *authorization, char *respdata );
void http_head_parser( const char *headData, char *parseVal, char *keyVal );

#endif /* WLAN_MWM_H_ */
