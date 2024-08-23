#ifndef DATETIME_PARSER_H_
#define DATETIME_PARSER_H_

#include "datetime_types.h"

#include <stdbool.h>

bool getDatetimeFromDateHeader( const char *datetimeStr, Datetime_t *datetime );
bool getLocalDatetimeFromDateHeader( const char *datetimeStr, Datetime_t *datetime );

#endif /* DATETIME_PARSER_H_ */
