#ifndef WEBPICTURE_API_H_
#define WEBPICTURE_API_H_

#include "http_return_codes.h"
#include "picture_management/picture_types.h"

#include <stdbool.h>

HttpReturnCodes_t httpGetCustomPicture( PictureInfo_t *customPicture, bool ignoreWasReadFlag );

#endif /* WEBPICTURE_API_H_ */
