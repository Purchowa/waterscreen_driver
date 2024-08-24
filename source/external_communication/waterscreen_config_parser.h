#ifndef WATERSCREEN_CONFIG_PARSER_H_
#define WATERSCREEN_CONFIG_PARSER_H_

#include "config/waterscreen_config.h"

#include <stdbool.h>

/**
 *
 * @param cfgJson source of raw JSON as null-terminated string
 * @param config destinantion for the parsed config data
 * @return false if parsing failed (`config` might be left in invalid state).
 * @return true if parsing was successful
 */
bool fromJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config );

#endif /* WATERSCREEN_CONFIG_PARSER_H_ */
