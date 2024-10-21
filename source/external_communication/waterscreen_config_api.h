#ifndef WATERSCREEN_CONFIG_API_H_
#define WATERSCREEN_CONFIG_API_H_

#include "http_return_codes.h"
#include "config/waterscreen_config.h"

#include <stdbool.h>


HttpReturnCodes_t getWaterscreenConfigFromApi( WaterscreenConfig_t *config, bool ignoreWasReadFlag );

#endif /* WATERSCREEN_CONFIG_API_H_ */
