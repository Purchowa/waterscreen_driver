#include "datetime_api.h"

#include "datetime/datetime_parser.h"

#include "config/wifi_cfg.h"
#include "external/wlan/wlan_mwm.h"
#include "http_get_utils.h"

#include <string.h>

#define DATE_STR_LEN 32


HttpReturnCodes_t httpGetDatetime( Datetime_t *datetime )
{
    char *httpGetBuffer = getClearedHttpGETBuffer();

    http_GET( WEATHER_API_URL, NULL, httpGetBuffer );

    size_t requestLen = strlen( httpGetBuffer );
    if ( requestLen == HTTP_HEAD_CONVERSION_FAILED_VALUE )
        return Http_GETError;

    char dateStr[DATE_STR_LEN] = { 0 };
    http_head_parser( httpGetBuffer, dateStr, "Date: " );

    if ( !getLocalDatetimeFromDateHeader( dateStr, datetime ) )
        return Http_DatetimeParsingError;

    return Http_Success;
}
