#ifndef DATETIME_MOCKS_H
#define DATETIME_MOCKS_H

#include "datetime/rtc_provider.h"


void       will_return_datetime( const Datetime_t datetime );
Datetime_t getRTCDatetime();

#endif //DATETIME_MOCKS_H
