#include "webpicture_parser.h"

#include "picture_management/picture_types.h"
#include "neopixels/color.h"
#include "common_utils.h"
#include "http_return_codes.h"

#include "cjson/cjson.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_COLOR_CHANNEL_VALUE 255

static bool parseRGBColor( const cJSON *obj, ColorRGB_t *color )
{
    const cJSON *r = cJSON_GetObjectItemCaseSensitive( obj, "r" );
    if ( !cJSON_IsNumber( r ) )
        return false;

    color->r = clamp( r->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    const cJSON *g = cJSON_GetObjectItemCaseSensitive( obj, "g" );
    if ( !cJSON_IsNumber( g ) )
        return false;

    color->g = clamp( g->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    const cJSON *b = cJSON_GetObjectItemCaseSensitive( obj, "b" );
    if ( !cJSON_IsNumber( b ) )
        return false;

    color->b = clamp( b->valueint, 0, MAX_COLOR_CHANNEL_VALUE );

    return true;
}

static bool parsePictureObject( const cJSON *pictureObject, PictureInfo_t *const customPicture )
{
    if ( !cJSON_IsObject( pictureObject ) )
        return false;

    const cJSON *pictureSize = cJSON_GetObjectItemCaseSensitive( pictureObject, "size" );
    if ( !cJSON_IsNumber( pictureSize ) )
        return false;
    customPicture->picture.size = clamp( pictureSize->valueint, 0, MAX_CUSTOM_PICTURE_HEIGHT );

    const cJSON *pictureArray = cJSON_GetObjectItemCaseSensitive( pictureObject, "data" );
    if ( !cJSON_IsArray( pictureArray ) )
        return false;

    const cJSON *element  = NULL;
    uint32_t     i        = 0;
    pictureRow_t rowValue = 0;
    cJSON_ArrayForEach( element, pictureArray )
    {
        if ( !cJSON_IsString( element ) )
            return false;

        if ( customPicture->picture.size <= i )
            break;

        char *endptr;
        rowValue = strtoull( element->valuestring, &endptr, 10 );
        if ( *endptr != '\0' )
            return false;

        customPicture->picture.data[i++] = rowValue;
    }

    if ( i < customPicture->picture.size )
        return false;

    const cJSON *colorsObj = cJSON_GetObjectItemCaseSensitive( pictureObject, "colors" );
    if ( !cJSON_IsObject( colorsObj ) )
        return false;

    const cJSON *mainColorObj = cJSON_GetObjectItemCaseSensitive( colorsObj, "main" );
    if ( !cJSON_IsObject( mainColorObj ) )
        return false;

    if ( !parseRGBColor( mainColorObj, &customPicture->colors.main ) )
        return false;

    const cJSON *secondaryColorObj = cJSON_GetObjectItemCaseSensitive( colorsObj, "secondary" );
    if ( !cJSON_IsObject( secondaryColorObj ) )
        return false;

    if ( !parseRGBColor( secondaryColorObj, &customPicture->colors.secondary ) )
        return false;

    return true;
}

HttpReturnCodes_t parseJsonToCustomPicture( const char *jsonStr, PictureInfo_t *customPicture )
{
    cJSON *jsonObj = cJSON_Parse( jsonStr );

    const HttpReturnCodes_t code =
        parsePictureObject( jsonObj, customPicture ) ? Http_Success : Http_WebPictureParsingError;

    cJSON_Delete( jsonObj );

    return code;
}

static bool parseWasReadObject( const cJSON *jsonObj, bool *const wasRead )
{
    if ( !cJSON_IsObject( jsonObj ) )
        return false;

    const cJSON *wasReadField = cJSON_GetObjectItemCaseSensitive( jsonObj, "wasRead" );
    if ( !cJSON_IsBool( wasReadField ) )
        return false;

    *wasRead = wasReadField->valueint;

    return true;
}

HttpReturnCodes_t parseJsonToWasReadFlag( const char *jsonStr, bool *wasRead )
{
    cJSON *jsonObj = cJSON_Parse( jsonStr );

    const HttpReturnCodes_t code =
        parseWasReadObject( jsonObj, wasRead ) ? Http_Success : Http_WebPictureFlagParsingError;

    cJSON_Delete( jsonObj );

    return code;
}
