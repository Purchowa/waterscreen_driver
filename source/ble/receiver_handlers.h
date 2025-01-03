#ifndef RECEIVER_HANDLERS_H_
#define RECEIVER_HANDLERS_H_

#include <stdint.h>
#include <stdbool.h>


void handleBLENotifyEvents( const char *event, bool *isClientConnected );
void handleBLERTModeEvent( bool isRTModeActive );
void handleCustomPictureEvent();

#endif /* RECEIVER_HANDLERS_H_ */
