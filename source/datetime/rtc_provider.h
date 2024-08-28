#ifndef RTC_PROVIDER_H_
#define RTC_PROVIDER_H_

#include "datetime/datetime_types.h"

void initRTC();
void setDatetime( const Datetime_t *datetime );
void getDatetime( Datetime_t *datetime );

#endif /* RTC_PROVIDER_H_ */
