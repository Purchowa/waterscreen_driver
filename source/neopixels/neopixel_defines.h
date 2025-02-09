#ifndef NEOPIXEL_DEFINES_H_
#define NEOPIXEL_DEFINES_H_

#include <stdint.h>

#ifndef NEOPIXEL_COUNT
#define NEOPIXEL_COUNT 119     // count of unique NeoPixels
#endif
#define NEOPIXEL_STRIP_COUNT 2 // Colors on each strip will be a copy of itself
#define TOTAL_NEOPIXEL_COUNT ( NEOPIXEL_STRIP_COUNT * NEOPIXEL_COUNT )

#define MAIN_COLOR_FACTOR      1.0
#define SECONDARY_COLOR_FACTOR 0.8

#endif /* NEOPIXEL_DEFINES_H_ */
