#ifndef WATERSCREEN_STATE_SERIALIZER_H_
#define WATERSCREEN_STATE_SERIALIZER_H_

#include "waterscreen_status.h"

/**
 * @param status - struct to serialize into json format.
 * @return char* - owning pointer to a json string. User must free this resource manually.
 * @return NULL - on serialization error.
 */
char *serializeWaterscreenStatus( const WaterscreenStatus_t *status );

#endif /* WATERSCREEN_STATE_SERIALIZER_H_ */
