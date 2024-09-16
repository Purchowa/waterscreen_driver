#ifndef PICTURE_LOGIC_H_
#define PICTURE_LOGIC_H_

#include "picture_data.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "datetime/datetime_types.h"

const PictureDataView_t *getEachPictureView();
const PictureDataView_t *getOccasionalPictureView( const Datetime_t *, const WeatherCondition_t );

CustomPictureDataSpan_t *getCustomPictureSpan();

uint8_t getLastPictureIndex( const PictureDataView_t *picture );

#endif /* PICTURE_LOGIC_H_ */
