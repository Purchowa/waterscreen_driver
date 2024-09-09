#ifndef PICTURE_LOGIC_H_
#define PICTURE_LOGIC_H_

#include "picture_data.h"

const pictureDataView_t *getEachPictureView();
const pictureDataView_t *getOccasionalPictureView( const Date_t *, const WeatherCondition_t );

customPictureDataSpan_t *getCustomPictureSpan();

#endif /* PICTURE_LOGIC_H_ */
