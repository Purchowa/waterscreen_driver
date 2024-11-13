#include "http_utils.h"

#include "external/wlan/wlan_mwm.h"

#include <stdlib.h>
#include <string.h>


#define HTTP_GET_BUFFER_LEN  2046
#define CONTENT_LEN_BUFF_LEN 32

#define HTTP_CODE_BUFF_LEN 64
#define HTTP_CODE_PREFIX   "HTTP/1.1"

#define BASE_10 10

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

    char contentLenStr[CONTENT_LEN_BUFF_LEN] = { 0 };
    http_head_parser( httpGetBuffer, contentLenStr, "Content-Length:" );

    size_t contentLen = atol( contentLenStr );
    if ( contentLen == HTTP_HEAD_CONVERSION_FAILED_VALUE )
        return false;

    *destContent = httpGetBuffer + ( requestLen - contentLen );

    return true;
}

bool extractHttpResponseCode( const char *response, long *responseCode )
{
    char httpCodeStr[HTTP_CODE_BUFF_LEN] = { 0 };
    http_head_parser( response, httpCodeStr, HTTP_CODE_PREFIX );

    char *end;
    *responseCode = strtol( httpCodeStr, &end, BASE_10 );

    const bool wasParsed = end != response;
    return wasParsed;
}
