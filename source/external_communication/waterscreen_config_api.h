#ifndef WATERSCREEN_CONFIG_API_H_
#define WATERSCREEN_CONFIG_API_H_

#include "http_return_codes.h"
#include "config/waterscreen_config.h"

HttpReturnCodes_t getWaterscreenConfigFromApi( WaterscreenConfig_t *config );

#endif /* WATERSCREEN_CONFIG_API_H_ */
