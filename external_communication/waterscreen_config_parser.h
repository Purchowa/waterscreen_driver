#ifndef WATERSCREEN_CONFIG_PARSER_H_
#define WATERSCREEN_CONFIG_PARSER_H_

#include "config/waterscreen_config.h"

#include <stdbool.h>


bool loadFromRawJsonIntoConfig( const char *cfgJson, WaterscreenConfig_t *config );

#endif /* WATERSCREEN_CONFIG_PARSER_H_ */
