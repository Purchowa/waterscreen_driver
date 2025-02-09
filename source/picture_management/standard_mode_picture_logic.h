#ifndef STANDARD_MODE_PICTURE_LOGIC_H_
#define STANDARD_MODE_PICTURE_LOGIC_H_

#include "picture_types.h"

#include "datetime/datetime_types.h"
#include "http/weather_types.h"


const PictureInfo_t *getOccasionalPicture( const Datetime_t *, const WeatherCondition_t );


#endif /* STANDARD_MODE_PICTURE_LOGIC_H_ */
