#ifndef RECEIVER_HANDLERS_H_
#define RECEIVER_HANDLERS_H_

#include "receiver_data_types.h"

#include <stdint.h>
#include <stdbool.h>


void handleBLERTModeEvent( bool isRTModeActive );
void handleCustomPictureEvent();
void handleConfiguration( const SerializedConfiguration_t * );

#endif /* RECEIVER_HANDLERS_H_ */
