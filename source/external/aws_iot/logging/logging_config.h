#ifndef LOGGING_CONFIG_H_
#define LOGGING_CONFIG_H_

#include "logging_levels.h"

#include "ble/ble_send_provider.h"
#include "fsl_debug_console.h"

#define LOG_LEVEL            LOG_DEBUG
#define REDIRECT_LOGS_TO_BLE 0

#if REDIRECT_LOGS_TO_BLE
#define LogHandler( msg ) bleTextWrite( msg )
#else
#define LogHandler( msg ) PRINTF( msg )
#endif

#endif /* LOGGING_CONFIG_H_ */
