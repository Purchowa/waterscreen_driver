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
    Http_UnknownError,
} HttpReturnCodes_t;

#endif /* HTTP_RETURN_CODES_H_ */
