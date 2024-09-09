#ifndef PICTURE_DATA_H_
#define PICTURE_DATA_H_

#include "picture_types.h"
#include "external_communication/weather_types.h"

#ifndef PICTURE_COUNT
#define PICTURE_COUNT 22
#endif

extern const pictureDataView_t g_allPictures[PICTURE_COUNT];
extern const pictureDataView_t g_weatherPictures[WEATHER_CONDITION_SIZE];

#endif /* PICTURE_DATA_H_ */
