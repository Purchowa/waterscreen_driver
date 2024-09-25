#ifndef STANDARD_MODE_PICTURE_GETTERS_H_
#define STANDARD_MODE_PICTURE_GETTERS_H_

#include "picture_management/picture_data_constants.h"
#include "picture_management/picture_types.h"

#include "external_communication/weather_types.h"
#include "datetime/datetime_types.h"


typedef enum
{
    PictureGetterLoop,
    PictureGetterEndLoop
} PictureGetterLoopStatus_t;

typedef PictureGetterLoopStatus_t ( *pictureGetterFun_t )( const PictureDataView_t **const, const Datetime_t *,
                                                           const WeatherCondition_t );

PictureGetterLoopStatus_t getCurrentTimeAsPicture( const PictureDataView_t **const, const Datetime_t *,
                                                   const WeatherCondition_t );

PictureGetterLoopStatus_t getWeatherAsPicture( const PictureDataView_t **const, const Datetime_t *,
                                               const WeatherCondition_t );

PictureGetterLoopStatus_t getSeasonalPicture( const PictureDataView_t **const, const Datetime_t *,
                                              const WeatherCondition_t );

PictureGetterLoopStatus_t getStandardModePicture( const PictureDataView_t **const, const Datetime_t *,
                                                  const WeatherCondition_t );


extern const pictureGetterFun_t g_pictureGetterFunctions[STANDARD_MODE_PICTURE_GETTER_COUNT];

#endif /* STANDARD_MODE_PICTURE_GETTERS_H_ */
