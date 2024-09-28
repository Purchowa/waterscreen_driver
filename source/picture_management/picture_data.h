#ifndef PICTURE_DATA_H_
#define PICTURE_DATA_H_

#include "picture_management/picture_data_constants.h"
#include "picture_management/picture_types.h"

#include "external_communication/weather_types.h"


extern const PictureDataView_t g_allPictures[ALL_PICTURE_COUNT];
extern const PictureDataView_t g_standardModePictures[STANDARD_MODE_STATIC_PICTURE_COUNT];
extern const PictureDataView_t g_weatherPictures[WEATHER_CONDITION_SIZE];

extern const pictureCharacter_t g_characterToPictureMap[CHARACTER_TO_PICTURE_MAP_SIZE][CHARACTER_TO_PICTURE_ROW_COUNT];
extern pictureRow_t             g_timePicture[CHARACTER_TO_PICTURE_ROW_COUNT];

extern const SeasonsInfo_t  g_seasonsInfo[SEASONS_COUNT];
extern const HolidaysInfo_t g_holidaysInfo[HOLIDAYS_COUNT];

#endif /* PICTURE_DATA_H_ */