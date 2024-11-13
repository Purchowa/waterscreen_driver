#ifndef PICTURE_LOGIC_UTILS_H_
#define PICTURE_LOGIC_UTILS_H_

#include "picture_management/picture_types.h"


int32_t getLastPictureIndex( const PictureDataSpan_t *picture );

size_t cyclicIncrement( const size_t index, const size_t limit );

#endif /* PICTURE_LOGIC_UTILS_H_ */
