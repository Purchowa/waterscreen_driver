#ifndef RTC_PROVIDER_H_
#define RTC_PROVIDER_H_

#include "datetime/datetime_types.h"

void initRTC();
void setRTCDatetime( const Datetime_t *datetime );
void getRTCDatetime( Datetime_t *datetime );

#endif /* RTC_PROVIDER_H_ */
