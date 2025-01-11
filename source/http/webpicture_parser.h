#ifndef WEBPICTURE_PARSER_H_
#define WEBPICTURE_PARSER_H_

#include "picture_management/picture_types.h"
#include "http_return_codes.h"
#include <stdbool.h>

HttpReturnCodes_t parseJsonToCustomPicture( const char *jsonStr, PictureInfo_t *customPicture );
HttpReturnCodes_t parseJsonToWasReadFlag( const char *jsonStr, bool *wasRead );

#endif /* WEBPICTURE_PARSER_H_ */
