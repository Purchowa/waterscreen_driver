#ifndef STATUS_LOGGING_H_
#define STATUS_LOGGING_H_

struct WaterscreenContext_t;
struct Weather_t;
struct Datetime_t;

void logWaterscreenStatus( const WaterscreenContext_t *context );
void logWlanStatus();
void logWeather( const Weather_t *weather );
void logDatetime( const Datetime_t *datetime );


#endif /* STATUS_LOGGING_H_ */
