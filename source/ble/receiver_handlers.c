#include "receiver_handlers.h"

#include <string.h>


#define STRINGS_EQUAL 0

#define CLIENT_CONNECTED_EVN   "OK+CONN"
#define CLIENT_CONNECTION_LOST "OK+LOST"

const size_t getNotifyEventsTextLength()
{
    return sizeof( CLIENT_CONNECTED_EVN );
}

void handleBLENotifyEvents( const char *event, bool *isClientConnected )
{
    if ( strcmp( event, CLIENT_CONNECTED_EVN ) == STRINGS_EQUAL )
    {
        *isClientConnected = true;
    }
    else if ( strcmp( event, CLIENT_CONNECTION_LOST ) == STRINGS_EQUAL )
    {
        *isClientConnected = false;
    }
}
