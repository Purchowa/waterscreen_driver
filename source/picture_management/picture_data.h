#ifndef PICTURE_DATA_H_
#define PICTURE_DATA_H_

#include "picture_management/picture_data_constants.h"
#include "picture_management/picture_types.h"

#include "external_communication/weather_types.h"


extern PictureInfo_t g_customPicture;

extern const PictureInfo_t *g_allPictures[ALL_PICTURE_COUNT];
extern PictureInfo_t       *g_standardModePictures[STANDARD_MODE_STATIC_PICTURE_COUNT];
extern PictureInfo_t       *g_weatherPictures[WEATHER_CONDITION_SIZE];

extern const pictureCharacter_t g_characterToPictureMap[CHARACTER_TO_PICTURE_MAP_SIZE][NUMBER_OF_ROWS_IN_PICTURE_CHARACTER];
extern pictureRow_t             g_timePicture[NUMBER_OF_ROWS_IN_PICTURE_CHARACTER];

extern SeasonalPicture_t      g_seasonsInfo[SEASONS_COUNT];
extern HolidaysPictureArray_t g_holidaysInfo[HOLIDAYS_COUNT];


#endif /* PICTURE_DATA_H_ */
