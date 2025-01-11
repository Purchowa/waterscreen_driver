#include "http_return_codes.h"

static const char *s_httpCodesNames[Http_ReturnCodes_Count] = { "Success",
                                                                "GET-Error",
                                                                "POST-Error",
                                                                "WeatherParsingError",
                                                                "DatetimeParsingError",
                                                                "WSConfigParsingError",
                                                                "WSConfigNoUpdate",
                                                                "WSStatusSerializationError",
                                                                "WebPictureNoUpdate",
                                                                "WebPictureParsingError",
                                                                "WebPictureFlagParsingError"
                                                                "UnknownError" };

const char *getHttpReturnCodeName( const HttpReturnCodes_t returnCode )
{
    return s_httpCodesNames[returnCode];
}
