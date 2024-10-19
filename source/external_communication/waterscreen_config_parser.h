#ifndef WATERSCREEN_CONFIG_PARSER_H_
#define WATERSCREEN_CONFIG_PARSER_H_

#include "config/waterscreen_config.h"

#include <stdbool.h>

typedef enum
{
    ConfigSuccess,
    ConfigNoUpdate,
    ConfigParsingError
} ConfigParsingCodes_t;

/**
 *
 * @param cfgJson source of raw JSON as null-terminated string
 * @param config destinantion for the parsed config data
 * @return `ConfigParsingError` if parsing failed (`config` might be left in invalid state).
 * @return `ConfigSuccess` if parsing was successful
 * @return `ConfigNoUpdate` if config doesn't need to be updated
 */
ConfigParsingCodes_t fromJsonToWaterscreenCfg( const char *cfgJson, WaterscreenConfig_t *config );

#endif /* WATERSCREEN_CONFIG_PARSER_H_ */
