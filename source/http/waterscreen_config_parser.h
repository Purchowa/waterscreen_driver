#ifndef WATERSCREEN_CONFIG_PARSER_H_
#define WATERSCREEN_CONFIG_PARSER_H_

#include "config/waterscreen_config.h"
#include "http_return_codes.h"

#include <stdbool.h>


/**
 * @param cfgJson - source of raw JSON as null-terminated string
 * @param config - destinantion for the parsed config data
 * @param isInitialRequest - tells whether to ignore 'wasRead' flag inside json or not
 * @return `Http_WaterscreenConfigParsingError` if parsing failed (`config` might be left in invalid state).
 * @return `Http_Success` if parsing was successful
 * @return `Http_WaterscreenConfigNoUpdate` if config doesn't need to be updated
 */
HttpReturnCodes_t parseJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config, bool isInitialRequest );

#endif /* WATERSCREEN_CONFIG_PARSER_H_ */
