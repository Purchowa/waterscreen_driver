#include "ble_send_provider.h"

#include "ble_sender.h"

#include <string.h>


bool bleTextWrite( const char *text )
{
    const size_t textLen = strlen( text );

    return bleWrite( (byte_t *)text, textLen );
}
