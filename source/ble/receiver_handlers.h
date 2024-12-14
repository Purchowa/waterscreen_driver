#ifndef RECEIVER_HANDLERS_H_
#define RECEIVER_HANDLERS_H_

#include <stdint.h>
#include <stdbool.h>


#define BLE_NOTIFY_EVN_LEN sizeof( CLIENT_CONNECTED_EVN )

const size_t getNotifyEventsTextLength();
void         handleBLENotifyEvents( const char *event, bool *isClientConnected );

#endif /* RECEIVER_HANDLERS_H_ */
