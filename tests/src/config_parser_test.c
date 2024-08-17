#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "external_communication/waterscreen_config_parser.h"

#include <string.h>

static bool compareConfigs( const WaterscreenConfig_t *first, const WaterscreenConfig_t *second )
{
    bool isEqual = true;

    isEqual &= first->mode == second->mode;
    isEqual &= first->isWorkingDuringWeekends == second->isWorkingDuringWeekends;
    isEqual &= first->workTimeInStandardMode == second->workTimeInStandardMode;
    isEqual &= first->idleTimeInStandardMode == second->idleTimeInStandardMode;
    isEqual &= first->customPictureSize == second->customPictureSize;

    if ( isEqual )
        isEqual &= !memcmp( first->customPicture, second->customPicture,
                            sizeof( *first->customPicture ) * first->customPictureSize );

    return isEqual;
}

void givenRawValidJson_configParser_fillConfigStructure( void ** )
{

    const char *sampleJson = "{\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
                             "4,\"data\":[0,0,24,1567]}}";
    const WaterscreenConfig_t expectedConfig = { .mode                    = Service,
                                                 .isWorkingDuringWeekends = true,
                                                 .workTimeInStandardMode  = 10,
                                                 .idleTimeInStandardMode  = 5,
                                                 .customPictureSize       = 4,
                                                 .customPicture           = { 0, 0, 24, 1567 } };

    WaterscreenConfig_t config;
    assert_true( loadFromRawJsonIntoConfig( sampleJson, &config ) );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidMode_configParser_returnFalse( void ** )
{
    WaterscreenConfig_t config;

    const char *invlaidMode_lower =
        "{\"mode\":-1,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";
    assert_false( loadFromRawJsonIntoConfig( invlaidMode_lower, &config ) );

    const char *invlaidMode_higher =
        "{\"mode\":3,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";
    assert_false( loadFromRawJsonIntoConfig( invlaidMode_higher, &config ) );
}

void givenInvalidSize_configParser_returnFalse( void ** )
{
    WaterscreenConfig_t config;

    const char *invlaidSize_lower =
        "{\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "-1,\"data\":[0,0,24,1567]}}";
    assert_false( loadFromRawJsonIntoConfig( invlaidSize_lower, &config ) );

    const char *invlaidSize_higher =
        "{\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "65,\"data\":[0,0,24,1567]}}";
    assert_false( loadFromRawJsonIntoConfig( invlaidSize_higher, &config ) );
}

int main()
{
    const struct CMUnitTest tests[] = { cmocka_unit_test( givenRawValidJson_configParser_fillConfigStructure ),
                                        cmocka_unit_test( givenInvlaidMode_configParser_returnFalse ),
                                        cmocka_unit_test( givenInvalidSize_configParser_returnFalse ) };

    return cmocka_run_group_tests_name( "JSON config parser", tests, NULL, NULL );
}
