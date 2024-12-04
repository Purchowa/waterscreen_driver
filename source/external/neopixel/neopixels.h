#ifndef NEOPIXELS_H_
#define NEOPIXELS_H_

#include <stdint.h>

typedef SPI_Type;

void Neopixels_Init( SPI_Type *base, uint32_t n );
void Neopixels_Send( uint32_t *value );
void Neopixels_Animate( uint32_t *value, uint32_t color );

#endif /* NEOPIXELS_H_ */
