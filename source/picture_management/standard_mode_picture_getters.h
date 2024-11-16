#ifndef STANDARD_MODE_PICTURE_GETTERS_H_
#define STANDARD_MODE_PICTURE_GETTERS_H_

#include "picture_management/picture_data_constants.h"
#include "picture_management/picture_types.h"

#include "external_communication/weather_types.h"
#include "datetime/datetime_types.h"


typedef enum
{
    PictureGetterLoop,
    PictureGetterEndLoop,
    NoAvailablePicture
} PictureGetterLoopStatus_t;

typedef PictureGetterLoopStatus_t ( *pictureGetterFun_t )( const PictureDataSpan_t **const, const Datetime_t *,
                                                           const WeatherCondition_t );

PictureGetterLoopStatus_t getCurrentTimeAsPicture( const PictureDataSpan_t **const, const Datetime_t *,
                                                   const WeatherCondition_t );

PictureGetterLoopStatus_t getWeatherAsPicture( const PictureDataSpan_t **const, const Datetime_t *,
                                               const WeatherCondition_t );

PictureGetterLoopStatus_t getSeasonalPicture( const PictureDataSpan_t **const, const Datetime_t *,
                                              const WeatherCondition_t );

PictureGetterLoopStatus_t getHolidaysPicture( const PictureDataSpan_t **const, const Datetime_t *,
                                              const WeatherCondition_t );

PictureGetterLoopStatus_t getCustomPicture( const PictureDataSpan_t **const, const Datetime_t *,
                                            const WeatherCondition_t );

PictureGetterLoopStatus_t getStandardModePicture( const PictureDataSpan_t **const, const Datetime_t *,
                                                  const WeatherCondition_t );


extern const pictureGetterFun_t g_pictureGetterFunctions[STANDARD_MODE_PICTURE_GETTER_COUNT];

PictureGetterLoopStatus_t callPictureGetterAtIndex( const size_t getterIndex, const PictureDataSpan_t **const picture,
                                                    const Datetime_t        *datetime,
                                                    const WeatherCondition_t weatherCondition );


#endif /* STANDARD_MODE_PICTURE_GETTERS_H_ */
