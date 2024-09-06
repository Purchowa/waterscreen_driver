#ifndef PICTURES_H_
#define PICTURES_H_

#include <stdint.h>

typedef struct
{
    const uint8_t   size;
    const uint64_t *data;
} pictureDataView_t;

/**
 * \return non-owning pointer to picture structure
 **/
const pictureDataView_t *getPicture();

#endif /* PICTURES_H_ */
