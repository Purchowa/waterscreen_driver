#include "standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"

#include "datetime/datetime_types.h"
#include "datetime/rtc_provider.h"

#include "gpio/power_control.h"

#include "external_communication/weather_api.h"

#include "picture_management/picture_logic_utils.h"
#include "picture_management/standard_mode_picture_logic.h"

#include <assert.h>


static StandardModeConfig_t s_standardModeCfg = { .isWorkingDuringWeekends = true,
                                                  .workTimeInStandardMode  = 1,
                                                  .idleTimeInStandardMode  = 1,
                                                  .workRange               = { .from = 7, .to = 24 } };

void setStandardModeConfig( const StandardModeConfig_t *newCfg )
{
    assert( newCfg );
    s_standardModeCfg = *newCfg;
}

static inline bool isCurrentlyWeekend( const Date_t *date )
{
    return ( Saturday == date->weekday || date->weekday == Sunday );
}

static inline bool isTimeInWorkRange( const Time_t *time )
{
    return ( s_standardModeCfg.workRange.from <= time->hour && time->hour <= s_standardModeCfg.workRange.to );
}

static inline bool isIdleTime( const Time_t *time )
{
    return ( time->minute % ( s_standardModeCfg.workTimeInStandardMode + s_standardModeCfg.idleTimeInStandardMode ) +
                 1 <=
             s_standardModeCfg.workTimeInStandardMode );
}

static inline bool isWorkingConditionSatisfied( const Datetime_t *datetime )
{
    return ( !isCurrentlyWeekend( &datetime->date ) || s_standardModeCfg.isWorkingDuringWeekends ) &&
        isTimeInWorkRange( &datetime->time ) && !isIdleTime( &datetime->time );
}

static Weather_t requestWeather()
{
    Weather_t weather;

    const WeatherApiStatusCode_t code = getWeather( &weather );
    if ( code != Success )
    {
        const Weather_t defaultWeather = { .temperature = 0.f, .pressure = 1000, .weatherCondition = Clouds };
        return defaultWeather;
    }

    return weather;
}

void standardModeState( WaterscreenContext_t *context )
{
    const Datetime_t datetime = getRTCDatetime();

    if ( !isWorkingConditionSatisfied( &datetime ) )
    {
        manageValvePower( OffDeviceState );
        return;
    }

    const Weather_t weather =
        requestWeather(); // TODO: requesting weather every working hour would be better for network

    context->pictureView      = getOccasionalPictureView( &datetime, weather.weatherCondition );
    context->valveOpenCounter = getLastPictureIndex( context->pictureView );

    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
}
