#ifndef WIFICFG_EXAMPLE_H_
#define WIFICFG_EXAMPLE_H_

/*-----------------------------------------------------------------------------
AP_SECURITY_MODE:
0 - Open
1 - WEP (Open mode)
2 - WEP (Shared mode)
3 - WPA-PSK
4 - WPA2-PSK
9 - WPA3-SAE
 ----------------------------------------------------------------------------*/

#define AP_SSID          ""
#define AP_PASSPHRASE    ""
#define AP_SECURITY_MODE ""

#define WEATHER_API_URL "api.openweathermap.org/data/2.5/weather?q=<location>,pl&APPID=<your_appid>&units=metric"

#define WATERSCREEN_API_BASIC_AUTH_BASE64       ""
#define WATERSCREEN_CONFIG_GET_URL              ""
#define WATERSCREEN_WEBPICTURE_GET_URL          ""
#define WATERSCREEN_WAS_WEBPICTURE_READ_GET_URL ""
#define WATERSCREEN_STATUS_POST_URL             ""

#define WIFI_CREDENTIALS_CAPACITY 128 // Capacity includes EOL

typedef struct
{
    char login[WIFI_CREDENTIALS_CAPACITY];
    char password[WIFI_CREDENTIALS_CAPACITY];
} WiFiCredentials_t;

extern WiFiCredentials_t g_wifiCredentials;

#endif /* WIFICFG_EXAMPLE_H_ */
