#include "webpicture_api.h"


#include "webpicture_parser.h"

#include "config/wifi_cfg.h"
#include "http_utils.h"

#include <stdlib.h>
#include <assert.h>

static HttpReturnCodes_t httpGetWasReadFlag()
{
    bool       wasRead = false;
    char      *content = NULL;
    const bool wasReceived =
        httpGET_receiveContent( &content, WATERSCREEN_API_BASIC_AUTH_BASE64, WATERSCREEN_WAS_WEBPICTURE_READ_GET_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( content );
    const HttpReturnCodes_t code = parseJsonToWasReadFlag( content, &wasRead );

    return wasRead ? Http_WebPictureNoUpdate : code;
}

HttpReturnCodes_t httpGetCustomPicture( PictureInfo_t *customPicture, bool ignoreWasReadFlag )
{
    const HttpReturnCodes_t code = httpGetWasReadFlag();

    if ( code == Http_Success || ( code == Http_WebPictureNoUpdate && ignoreWasReadFlag ) )
    {
        char      *pictureContent = NULL;
        const bool wasReceived    = httpGET_receiveContent( &pictureContent, WATERSCREEN_API_BASIC_AUTH_BASE64,
                                                            WATERSCREEN_WEBPICTURE_GET_URL );

        if ( !wasReceived )
            return Http_GETError;

        assert( pictureContent );
        return parseJsonToCustomPicture( pictureContent, customPicture );
    }

    return code;
}
