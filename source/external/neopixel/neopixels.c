#include "fsl_spi.h"
#include "neopixels.h"

#include <assert.h>

#define GET_BIT( k, n ) ( k & ( 1 << ( n ) ) )
#define SET_BIT( k, n ) ( k |= ( 1 << ( n ) ) )
#define CLR_BIT( k, n ) ( k &= ~( 1 << ( n ) ) )
#define CODE_0          0b10000
#define CODE_1          0b11100

SPI_Type *SPI_base   = NULL;
uint32_t  ledsNumber = 0;

void Neopixels_Init( SPI_Type *base, uint32_t n )
{
    SPI_base   = base;
    ledsNumber = n;
}

void Neopixels_Send( uint32_t *value )
{
    assert( SPI_base );

    uint16_t LED_data    = 0;
    uint32_t configFlags = 0U;

    /* ignore RX */
    configFlags |= ( 1 << 22 );

    for ( int j = 0; j < ledsNumber; j++ )
    {
        for ( int i = 23; i >= 0; i-- )
        {
            LED_data = GET_BIT( value[j], i ) ? CODE_1 : CODE_0;
            while ( !( SPI_GetStatusFlags( SPI_base ) & kSPI_TxNotFullFlag ) )
                ;
            SPI_WriteData( SPI_base, LED_data, configFlags );
        }
    }
    // Reset >= 50 us
    LED_data = 0;
    for ( int j = 0; j < 60; j++ )
    {
        while ( !( SPI_GetStatusFlags( SPI_base ) & kSPI_TxNotFullFlag ) )
            ;
        SPI_WriteData( SPI_base, LED_data, configFlags );
    }
}
void Neopixels_Animate( uint32_t *value, uint32_t color )
{
    assert( SPI_base );

    static uint32_t k = 0;
    for ( int j = 0; j < ledsNumber; j++ )
    {
        value[j] = 0; // leds off
    }
    value[k++] = color;
    if ( k >= ledsNumber )
    {
        k = 0;
    }
}
