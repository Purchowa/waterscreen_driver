#include "standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"

#include "datetime/datetime_types.h"
#include "datetime/rtc_provider.h"

#include "gpio/power_control.h"

#include "external_communication/weather_api.h"

#include "picture_management/picture_logic_utils.h"
#include "picture_management/standard_mode_picture_logic.h"

#include <assert.h>


#define WORK_RANGE_FROM 7
#define WORK_RANGE_TO   18

static StandardModeConfig_t s_standardModeCfg = {
    .isWorkingDuringWeekends = false, .workTimeInStandardMode = 1, .idleTimeInStandardMode = 1 };

void setStandardModeConfig( const StandardModeConfig_t *newCfg )
{
    assert( newCfg );
    s_standardModeCfg = *newCfg;
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
    return ( time->minute % ( s_standardModeCfg.workTimeInStandardMode + s_standardModeCfg.idleTimeInStandardMode ) +
                 1 <=
             s_standardModeCfg.workTimeInStandardMode );
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

    if ( s_standardModeCfg.isWorkingDuringWeekends && !isCurrentlyWeekend( &datetime.date ) )
        return;

    if ( !isTimeInWorkRange( &datetime.time ) )
        return;

    if ( isIdleTime( &datetime.time ) )
        return;

    const Weather_t weather = requestWeather();

    context->pictureView      = getOccasionalPictureView( &datetime, weather.weatherCondition );
    context->valveOpenCounter = getLastPictureIndex( context->pictureView );
    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
}
