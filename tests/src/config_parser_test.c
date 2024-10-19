#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "external_communication/waterscreen_config_parser.h"

#include <string.h>

static bool compareConfigs( const WaterscreenConfig_t *first, const WaterscreenConfig_t *second )
{
    bool areEqual = true;

    areEqual &= first->mode == second->mode;
    areEqual &= first->standardModeConfig.isWorkingDuringWeekends == second->standardModeConfig.isWorkingDuringWeekends;
    areEqual &= first->standardModeConfig.workTimeInStandardMode == second->standardModeConfig.workTimeInStandardMode;
    areEqual &= first->standardModeConfig.idleTimeInStandardMode == second->standardModeConfig.idleTimeInStandardMode;
    areEqual &= first->customPictureSize == second->customPictureSize;

    if ( areEqual )
        areEqual &= !memcmp( first->customPicture, second->customPicture,
                             sizeof( *first->customPicture ) * first->customPictureSize );

    return areEqual;
}

void givenReadConfig_configParser_returnNoUpdate()
{
    WaterscreenConfig_t cfg;
    const char         *sampleJson =
        "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";

    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &cfg ), ConfigNoUpdate );
}

void givenRawValidJson_configParser_fillConfigStructure()
{

    const char *sampleJson =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";
    const WaterscreenConfig_t expectedConfig = { .mode = Service,
                                                 .standardModeConfig =
                                                     {
                                                         .isWorkingDuringWeekends = true,
                                                         .workTimeInStandardMode  = 10,
                                                         .idleTimeInStandardMode  = 5,
                                                     },
                                                 .customPictureSize = 4,
                                                 .customPicture     = { 0, 0, 24, 1567 } };

    WaterscreenConfig_t config;
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config ), ConfigSuccess );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidMode_configParser_returnFalse()
{
    WaterscreenConfig_t config;

    const char *invlaidMode_lower =
        "{\"wasRead\":false,\"mode\":-1,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invlaidMode_lower, &config ), ConfigParsingError );

    const char *invlaidMode_higher =
        "{\"wasRead\":false,\"mode\":3,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invlaidMode_higher, &config ), ConfigParsingError );
}

void givenArrayShorterThanSize_configParser_returnFalse()
{
    WaterscreenConfig_t config;

    const char *invalidArrayWithSize =
        "{\"wasRead\":false,\"mode\":0,\"enableWeekends\":true,\"workTime\":0,\"idleTime\":0,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24]}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invalidArrayWithSize, &config ), ConfigParsingError );
}

void givenInvalidSize_configParser_returnTrueAndClamp()
{
    WaterscreenConfig_t configLower;

    const char *invlaidSize_lower =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "-1,\"data\":[0,0,24,1567]}}";
    WaterscreenConfig_t expectedConfig = { .mode = Service,
                                           .standardModeConfig =
                                               {
                                                   .isWorkingDuringWeekends = true,
                                                   .workTimeInStandardMode  = 10,
                                                   .idleTimeInStandardMode  = 5,
                                               },
                                           .customPictureSize = 0,
                                           .customPicture     = { 0, 0, 24, 1567 } };

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_lower, &configLower ), ConfigSuccess );
    assert_true( compareConfigs( &expectedConfig, &configLower ) );

    WaterscreenConfig_t configHigher;
    const char         *invlaidSize_higher =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "65,\"data\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}}";

    expectedConfig.customPictureSize = MAX_CUSTOM_PICTURE_LENGTH;
    memset( expectedConfig.customPicture, 0,
            sizeof( *expectedConfig.customPicture ) * expectedConfig.customPictureSize );

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_higher, &configHigher ), ConfigSuccess );
    assert_true( compareConfigs( &expectedConfig, &configHigher ) );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenRawValidJson_configParser_fillConfigStructure ),
        cmocka_unit_test( givenInvlaidMode_configParser_returnFalse ),
        cmocka_unit_test( givenArrayShorterThanSize_configParser_returnFalse ),
        cmocka_unit_test( givenInvalidSize_configParser_returnTrueAndClamp ),
        cmocka_unit_test( givenReadConfig_configParser_returnNoUpdate ),
    };

    return cmocka_run_group_tests_name( "JSON config parser", tests, NULL, NULL );
}
