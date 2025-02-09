#include "standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

#include "datetime/datetime_types.h"
#include "datetime/rtc_provider.h"
#include "http/weather_api.h"

#include "picture_management/picture_logic_utils.h"
#include "picture_management/standard_mode_picture_logic.h"

#include "neopixels/neopixel_provider.h"

#include "gpio/power_control.h"

#include <stdlib.h>
#include <assert.h>


const static StandardModeConfig_t *s_standardModeCfg = NULL;

void initStandardModeConfig( const StandardModeConfig_t *newCfg )
{
    assert( newCfg );
    s_standardModeCfg = newCfg;
}

static inline bool isCurrentlyWeekend( const Date_t *date )
{
    return ( Saturday == date->weekday || date->weekday == Sunday );
}

static inline bool isTimeInWorkRange( const Time_t *time )
{
    return ( s_standardModeCfg->workRange.from <= time->hour && time->hour <= s_standardModeCfg->workRange.to );
}

static inline bool isIdleTime( const Time_t *time )
{
    const uint8_t timePosition =
        time->minute % ( s_standardModeCfg->workTimeInStandardMode + s_standardModeCfg->idleTimeInStandardMode ) + 1;

    return s_standardModeCfg->workTimeInStandardMode < timePosition;
}

static inline bool isWorkingConditionSatisfied( const Datetime_t *datetime )
{
    return ( !isCurrentlyWeekend( &datetime->date ) || s_standardModeCfg->isWorkingDuringWeekends ) &&
        isTimeInWorkRange( &datetime->time ) && !isIdleTime( &datetime->time );
}

void standardModeState( WaterscreenContext_t *context )
{
    assert( s_standardModeCfg );
    dimNeopixels();

    const Datetime_t datetime = getRTCDatetime();
    if ( !isWorkingConditionSatisfied( &datetime ) )
    {
        manageValvePower( OffDeviceState );
        checkSensorsSubState( context );
        return;
    }

    const Weather_t *weather = getWeather();

    context->pictureInfo      = getOccasionalPicture( &datetime, weather->weatherCondition );
    context->valveOpenCounter = getLastPictureIndex( &context->pictureInfo->picture );

    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
    context->stateDelay = PRESENTING_DELAY_MS;
}
