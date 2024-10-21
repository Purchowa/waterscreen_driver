#include "http_get_utils.h"

#include "external/wlan/wlan_mwm.h"

#include <stdlib.h>
#include <string.h>


#define HTTP_GET_BUFFER_LEN 2046
#define CONTENT_LEN_STR_LEN 32


static char s_httpGetBuffer[HTTP_GET_BUFFER_LEN];

char *getClearedHttpGETBuffer()
{
    memset( s_httpGetBuffer, 0, HTTP_GET_BUFFER_LEN );

    return s_httpGetBuffer;
}


bool httpGET_receiveContent( char **destContent, const char *authorization, const char *apiUrl )
{
    char *httpGetBuffer = getClearedHttpGETBuffer();

    http_GET( apiUrl, authorization, httpGetBuffer );

    size_t requestLen = strlen( httpGetBuffer );
    if ( requestLen == HTTP_HEAD_CONVERSION_FAILED_VALUE )
        return false;

    char contentLenStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( httpGetBuffer, contentLenStr, "Content-Length:" );

    size_t contentLen = atol( contentLenStr );
    if ( contentLen == HTTP_HEAD_CONVERSION_FAILED_VALUE )
        return false;

    *destContent = httpGetBuffer + ( requestLen - contentLen );

    return true;
}
