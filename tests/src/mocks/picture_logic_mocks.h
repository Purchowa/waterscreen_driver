#ifndef PICTURE_LOGIC_MOCKS_H
#define PICTURE_LOGIC_MOCKS_H

#include "picture_managment/demo_mode_picture_logic.h"


const PictureDataView_t *getEachPictureView();
const PictureDataView_t *getOccasionalPictureView( const Datetime_t *, const WeatherCondition_t );

#endif //PICTURE_LOGIC_MOCKS_H
