#ifndef WATERSCREEN_STATUS_API_H_
#define WATERSCREEN_STATUS_API_H_

#include "waterscreen_status.h"
#include "http_return_codes.h"


HttpReturnCodes_t httpPostWaterscreenStatus( const WaterscreenStatus_t * );

#endif /* WATERSCREEN_STATUS_API_H_ */
