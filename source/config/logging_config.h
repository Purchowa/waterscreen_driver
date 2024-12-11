#ifndef LOGGING_CONFIG_H_
#define LOGGING_CONFIG_H_

#include "logging_levels.h"
#include "fsl_debug_console.h"

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_DEBUG
#endif

#ifndef LogHandler
#define LogHandler( msg ) PRINTF( msg )
#endif

#include "logging.h"

#endif /* LOGGING_CONFIG_H_ */
