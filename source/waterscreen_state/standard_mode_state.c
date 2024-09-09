#include "standard_mode_state.h"

#include "datetime/datetime_types.h"
#include "datetime/rtc_provider.h"
#include "external_communication/weather_api.h"

#include <assert.h>

#define WORK_RANGE_FROM 7
#define WORK_RANGE_TO   18

static StandardModeConfig_t standardModeCfg = {
    .isWorkingDuringWeekends = false, .workTimeInStandardMode = 1, .idleTimeInStandardMode = 1 };

void setStandardModeConfig( const StandardModeConfig_t *newCfg )
{
    assert( newCfg );
    standardModeCfg = *newCfg;
}

static inline bool isCurrentlyWeekend( const Date_t *date )
{
    return ( Saturday == date->weekday && date->weekday == Sunday );
}

static inline bool isTimeInWorkRange( const Time_t *time )
{
    return ( WORK_RANGE_FROM <= time->hour && time->hour <= WORK_RANGE_TO );
}

static inline bool isIdleTime( const Time_t *time )
{
    return ( time->minute % ( standardModeCfg.workTimeInStandardMode + standardModeCfg.idleTimeInStandardMode ) + 1 <=
             standardModeCfg.workTimeInStandardMode );
}

static Weather_t requestWeather()
{
    Weather_t weather;

    // TODO: get that status code into context.currentStateStatus
    const WeatherApiStatusCode_t code = getWeather( &weather );
    if ( code != Success )
    {
        const Weather_t mockedWeather = { .temperature = 0.f, .pressure = 0, .weatherCondition = Clouds };
        return mockedWeather;
    }

    return weather;
}

void standardModeState( WaterscreenContext_t *context )
{
    const Datetime_t datetime = getRTCDatetime();

    if ( standardModeCfg.isWorkingDuringWeekends && !isCurrentlyWeekend( &datetime.date ) )
        return;

    if ( !isTimeInWorkRange( &datetime.time ) )
        return;

    if ( isIdleTime( &datetime.time ) )
        return;

    const Weather_t weather = requestWeather();
}
