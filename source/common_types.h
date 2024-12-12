#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#include <stdint.h>


typedef struct
{
    float *data;
    size_t size;
} FloatSpan_t;

typedef uint8_t byte_t;

#endif /* COMMON_TYPES_H_ */
