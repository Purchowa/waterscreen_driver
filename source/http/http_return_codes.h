#ifndef HTTP_RETURN_CODES_H_
#define HTTP_RETURN_CODES_H_


typedef enum
{
    Http_Success,
    Http_GETError,
    Http_POSTError,
    Http_WeatherParsingError,
    Http_DatetimeParsingError,
    Http_WaterscreenConfigParsingError,
    Http_WaterscreenConfigNoUpdate,
    Http_WaterscreenStatusSerializationError,
    Http_WebPictureNoUpdate,
    Http_WebPictureParsingError,
    Http_WebPictureFlagParsingError,
    Http_UnknownError,

    Http_ReturnCodes_Count
} HttpReturnCodes_t;

const char *getHttpReturnCodeName( const HttpReturnCodes_t returnCode );

#endif /* HTTP_RETURN_CODES_H_ */
